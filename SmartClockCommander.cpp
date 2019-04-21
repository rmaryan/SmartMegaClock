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
#include <errno.h>
#include "log.h"
#include "SmartClockConfig.h"
#include "SmartClockUI.h"
#include "SmartClockSensors.h"

#define CMD_MAX_STRING 200
const char* CMD_ERROR_TL = "Command is too long...";

char SmartClockCommander::inputString0 [CMD_MAX_STRING] = "";
char SmartClockCommander::inputString1 [CMD_MAX_STRING] = "";

HardwareSerial* SmartClockCommander::serial0 = 0;
HardwareSerial* SmartClockCommander::serial1 = 0;

char SmartClockCommander::commandString [CMD_MAX_STRING] = "";
char SmartClockCommander::commandParam1 [CMD_MAX_STRING] = "";
char SmartClockCommander::commandParam2 [CMD_MAX_STRING] = "";


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
		char* command) {
	const char* CMD_FONTS = "fonts";
	const char* CMD_TIME = "time";
	const char* CMD_DATE = "date";
	const char* CMD_SHOWDATE = "showdate";
	const char* CMD_COLOR = "color";
	const char* CMD_COLOR_TIME = "time";
	const char* CMD_COLOR_DATE = "date";
	const char* CMD_COLOR_HUM = "hum";
	const char* CMD_COLOR_TEMP = "temp";
	const char* CMD_RESET = "reset";
	const char* CMD_EQUALS = " = ";
	const char* CMD_EQUALS_HEX = " = 0x";
	const char* CMD_HELP = "help";
	const char* CMD_HELP_TEXT =
			"fonts [X] - select the font preset #X (X is a number from 1 to 5). If no parameter specified - current font # is shown.\n\
\n\
time [HH:MM] - set the clock time (in 24-hours format). If no parameter specified - current time is shown.\n\
\n\
date [DD/MM/YYY] - set the clock date. If no parameter specified - current date is shown.\n\
\n\
showdate [Y | N] - turns the date indication on or off. If no parameter specified - current state is shown.\n\
\n\
color [time | date | hum | temp  0x123456] - set the RGB color for the area specified.\n\
Please note, RGB colors are converted to 16-bit RGB565 format (since our screen works with the 16-bit colors only).\n\
If no parameter specified - current colors are listed.\n\
\n\
reset - reset colors and showdate flag to the factory default state. The configuration file is wiped.\n\
\n\
help - show this help information.";

	const char* CMD_ERROR_UNKN = "Command is unknown";
	const char* CMD_ERROR_PARAM_UNKN = "Can't process parameter";

	parseCommand(command);

	// do actual commands processing
	if(strcmp(commandString, CMD_FONTS)==0) {
		// show current status
		if(!commandParam1[0]) {
			in_serial->print(CMD_FONTS);
			in_serial->print(CMD_EQUALS);
			in_serial->println(SmartClockConfig::cnfFontPreset);
		} else {
			errno = 0;
			long longValue = strtol(commandParam1, NULL, 10);
			if(errno || (longValue < 0) || (longValue > 4)) {
				in_serial->println(CMD_ERROR_PARAM_UNKN);
			} else {
				SmartClockConfig::cnfFontPreset = longValue;
				SmartClockConfig::saveConfig();
				SmartClockUI::setForceRefresh();
			}
		}
	} else if(strcmp(commandString, CMD_TIME)==0) {
		if(!commandParam1[0]) {
			bool h12;
			bool PM;
			in_serial->print(SmartClockSensors::clock.getHour(h12, PM));
			in_serial->print(":");
			in_serial->print(SmartClockSensors::clock.getMinute());
			in_serial->print(":");
			in_serial->print(SmartClockSensors::clock.getSecond());
		} else {
			// TODO SET TIME
		}
	} else if(strcmp(commandString, CMD_DATE)==0) {
		if(!commandParam1[0]) {
			bool Century = false;
			in_serial->print(SmartClockSensors::clock.getDate());
			in_serial->print("/");
			in_serial->print(SmartClockSensors::clock.getMonth(Century));
			in_serial->print("/");
			in_serial->print(SmartClockSensors::clock.getYear());
		} else {
			// TODO SET DATE
			SmartClockUI::setForceRefresh();
		}
	} else if(strcmp(commandString, CMD_SHOWDATE)==0) {
		if(!commandParam1[0]) {
			in_serial->print("Showing date = ");
			in_serial->println(SmartClockConfig::cnfShowDate?"Y":"N");
		} else {
			// TODO SET SHOW DATE

			SmartClockConfig::saveConfig();
			SmartClockUI::setForceRefresh();
		}
	} else if(strcmp(commandString, CMD_COLOR)==0) {
		if(!commandParam1[0]) {
			in_serial->print(CMD_COLOR_TIME);
			in_serial->print(CMD_EQUALS_HEX);
			in_serial->println(SmartClockConfig::cnfColorTime, HEX);
			in_serial->print(CMD_COLOR_DATE);
			in_serial->print(CMD_EQUALS_HEX);
			in_serial->println(SmartClockConfig::cnfColorDate, HEX);
			in_serial->print(CMD_COLOR_HUM);
			in_serial->print(CMD_EQUALS_HEX);
			in_serial->println(SmartClockConfig::cnfColorHum, HEX);
			in_serial->print(CMD_COLOR_TEMP);
			in_serial->print(CMD_EQUALS_HEX);
			in_serial->println(SmartClockConfig::cnfColorTemp, HEX);
		} else {
			// TODO SET COLOR
			// show message "Converted to..."

			SmartClockConfig::saveConfig();
			SmartClockUI::setForceRefresh();
		}
	} else if(strcmp(commandString, CMD_RESET)==0) {
		SmartClockConfig::setDefaults();
		SmartClockConfig::saveConfig();
		SmartClockUI::setForceRefresh();
	} else if(strcmp(commandString, CMD_HELP)==0) {
		in_serial->println(CMD_HELP_TEXT);
	} else {
		in_serial->println(CMD_ERROR_UNKN);
	}
}

void SmartClockCommander::init(HardwareSerial* in_serial0, HardwareSerial* in_serial1) {
	serial0 = in_serial0;
	serial1 = in_serial1;
}

void SmartClockCommander::parseCommand(char* command) {
	commandString[0] = 0;
	commandParam1[0] = 0;
	commandParam2[0] = 0;

	// start splitting the string
	char * pch = strtok (command," ");
	if(pch != NULL) {
		// copy command string
		strncat(commandString, pch, CMD_MAX_STRING);
		// look for parameters
		pch = strtok (NULL, " ");
		if(pch != NULL) {
			strncat(commandParam1, pch, CMD_MAX_STRING);
			pch = strtok (NULL, " ");
			if(pch != NULL) {
				strncat(commandParam2, pch, CMD_MAX_STRING);
			}

		}
	}
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
	if(stringLength>CMD_MAX_STRING) {
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
