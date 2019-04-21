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

#define SWITCHPIN 7 // switch is connected to the pin #7



DHT SmartClockSensors::dht(DHTPIN, DHTTYPE);
DS3231 SmartClockSensors::clock;

void SmartClockSensors::init() {
	dht.begin();
}
