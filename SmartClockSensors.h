/*
 * SmartClockSensors.h
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

#ifndef SMARTCLOCKSENSORS_H_
#define SMARTCLOCKSENSORS_H_

#include <DHT.h>
#include <DS3231.h>

class SmartClockSensors {
public:
	static DHT dht;
	static DS3231 clock;

	static void init();
	// increases or decreases current hour by the specified number
	static void adjustHour(int8_t delta);
	// increases or decreases current minute by the specified number
	static void adjustMinutes(int8_t delta);
};

#endif /* SMARTCLOCKSENSORS_H_ */
