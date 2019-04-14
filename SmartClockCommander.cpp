/*
 * SmartClockCommander.cpp
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

#include "SmartClockCommander.h"
#include "log.h"

#define CMD_MAX_STRING 200

const char* SmartClockCommander::CMD_ERROR_TL = "Command is too long...";

char SmartClockCommander::inputString0 [CMD_MAX_STRING] = "";
char SmartClockCommander::inputString1 [CMD_MAX_STRING] = "";

HardwareSerial* SmartClockCommander::serial0 = 0;
HardwareSerial* SmartClockCommander::serial1 = 0;

char* SmartClockCommander::strtrim(char* str) {
	// jump to the first non-empty character
	while(*str < 33) {
		if(*str == 0) return str;
		str++;
	}

	char * buffer = str;
	while (*++buffer);

	while(*buffer <33) buffer--;

	*(++buffer) = 0;

	return str;
}

void SmartClockCommander::processCommand(HardwareSerial* in_serial,
		const char* command) {

	in_serial->print("Received command: '");
	in_serial->print(command);
	in_serial->println("'");

	// TODO Handle commands here
}

void SmartClockCommander::init(HardwareSerial* in_serial0, HardwareSerial* in_serial1) {
	serial0 = in_serial0;
	serial1 = in_serial1;
}

void SmartClockCommander::feedCharacter(uint8_t serialID) {
	HardwareSerial* serial;
	char * inputString;

	if(serialID == 0) {
		serial = serial0;
		inputString = inputString0;
	} else if(serialID == 1) {
		serial = serial1;
		inputString = inputString1;
	} else {
		return;
	}

	size_t stringLength = strlen(inputString);

	// check for string overflow
	if(stringLength>150) {
		Serial.println(CMD_ERROR_TL);
		inputString[0] = 0;
	} else {
		// get the new byte
		char inChar = serial->read();
		// if the incoming character is a newline - process a complete command
		if (inChar == '\n') {
			processCommand(serial, strtrim(inputString));
			inputString[0] = 0;
		} else {
			// add it to the inputString
			inputString[stringLength] = inChar;
			inputString[stringLength+1] = 0;
		}
	}
}
