/*
 * SmartClockSensors.cpp
 *
 *  Copyright (c) 2019 Maryan Rachynskyy
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "SmartClockSensors.h"

// Initialize DHT sensor
#define DHTPIN 17     // DHT sensor is connected to the pin #17
#define DHTTYPE DHT22 // DHT type is DHT22

#define DS3231_I2C_ADDRESS 0x68 // default DS3231 address

DHT SmartClockSensors::dht(DHTPIN, DHTTYPE);
DS3231 SmartClockSensors::clock;

void SmartClockSensors::init() {
	dht.begin();

	// sometimes the DS3231 stop progressing time when in power
	// let's make sure it is not the case
	Wire.begin();
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0xE);  // control register address
	Wire.write(0x00); // make sure 7th bit is zero
	Wire.endTransmission();
}

void SmartClockSensors::adjustHour(int8_t delta) {
	bool h12;
	bool PM;

	int8_t hour = SmartClockSensors::clock.getHour(h12, PM);

	hour+=delta;

	if(hour > 23) {
		hour = hour - 24;
	} else
		if(hour < 0) {
			hour = 24 + hour;
		}
	SmartClockSensors::clock.setHour(hour);
}

void SmartClockSensors::adjustMinutes(int8_t delta) {

	int8_t minutes = SmartClockSensors::clock.getMinute();

	minutes+=delta;

	if(minutes>59) {
		minutes = minutes - 60;
	} else
		if(minutes < 0) {
			minutes = 60 + minutes;
		}

	SmartClockSensors::clock.setMinute(minutes);
}
