/*
 * SmartClockCommander.h
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

#ifndef SMARTCLOCKCOMMANDER_H_
#define SMARTCLOCKCOMMANDER_H_

#include <HardwareSerial.h>

/**
 * A smart clock commands processing class.
 * Intended to handle two serial streams (supposedly primary serial + Wifi serial)
 */
class SmartClockCommander {
private:
	static const char* CMD_ERROR_TL;  // Command too long error message
	static char inputString0 [];      // a String to hold incoming data
	static char inputString1 [];      // a String to hold incoming data
	static HardwareSerial* serial0;
	static HardwareSerial* serial1;

	static char* strtrim(char* str); // a helper method which trims the string
	static void processCommand(HardwareSerial* in_serial, const char* command); // the command processing core

public:
	static void init(HardwareSerial* in_serial0, HardwareSerial* in_serial1);
	static void feedCharacter(uint8_t serialID); // receive character from a serial stream and handle it
};

#endif /* SMARTCLOCKCOMMANDER_H_ */
