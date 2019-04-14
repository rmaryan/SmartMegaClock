/*
 * SmartClockConfig.cpp
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

#include "SmartClockConfig.h"

#include <errno.h>

#include "log.h"

SDConfigFile SmartClockConfig::sdCnfFile;
const char * SmartClockConfig::CNF_FILE_NAME = "settings.txt";
const char * SmartClockConfig::CNF_KEY_FONTS = "fonts";
const char * SmartClockConfig::CNF_KEY_SHOWDATE = "showdate";
const char * SmartClockConfig::CNF_KEY_COLOR_TIME = "color_time";
const char * SmartClockConfig::CNF_KEY_COLOR_DATE = "color_date";
const char * SmartClockConfig::CNF_KEY_COLOR_HUM = "color_hum";
const char * SmartClockConfig::CNF_KEY_COLOR_TEMP = "color_temp";
uint8_t  SmartClockConfig::configFontPreset;
bool     SmartClockConfig::cnfShowDate;
uint16_t SmartClockConfig::cnfColorTime;
uint16_t SmartClockConfig::cnfColorDate;
uint16_t SmartClockConfig::cnfColorHum;
uint16_t SmartClockConfig::cnfColorTemp;


void SmartClockConfig::loadConfig() {
	// set the configuration defaults
	configFontPreset = CNF_DEFAULT_FONT_PRESET;
	cnfShowDate = CNF_DEFAULT_SHOW_DATE;
	cnfColorTime = CNF_DEFAULT_COLOR_TIME;
	cnfColorDate = CNF_DEFAULT_COLOR_DATE;
	cnfColorHum = CNF_DEFAULT_COLOR_HUM;
	cnfColorTemp = CNF_DEFAULT_COLOR_TEMP;

	// try to open a config file
	if (!sdCnfFile.begin(CNF_FILE_NAME, CNF_MAX_LINE_LENGHT)) {
		LOGGER_PRINT("Failed to open configuration file: ");
		LOGGER_PRINTLN(CNF_FILE_NAME);
		return;
	}

	// Read each setting from the file.
	while (sdCnfFile.readNextSetting()) {
		if (sdCnfFile.nameIs(CNF_KEY_FONTS)) {
			int intValue = sdCnfFile.getIntValue();
			if(intValue > 0) {
				configFontPreset = intValue;
			}
		} else if (sdCnfFile.nameIs(CNF_KEY_SHOWDATE)) {
			cnfShowDate = sdCnfFile.getBooleanValue();
		} else if (sdCnfFile.nameIs(CNF_KEY_COLOR_TIME)) {
			long longValue = sdCnfFile.getLongValue(16);
			if(errno == 0) {
				cnfColorTime = longValue;
			}
		} else if (sdCnfFile.nameIs(CNF_KEY_COLOR_DATE)) {
			long longValue = sdCnfFile.getLongValue(16);
			if(errno == 0) {
				cnfColorDate = longValue;
			}
		} else if (sdCnfFile.nameIs(CNF_KEY_COLOR_HUM)) {
			long longValue = sdCnfFile.getLongValue(16);
			if(errno == 0) {
				cnfColorHum = longValue;
			}
		} else if (sdCnfFile.nameIs(CNF_KEY_COLOR_TEMP)) {
			long longValue = sdCnfFile.getLongValue(16);
			if(errno == 0) {
				cnfColorTemp = longValue;
			}
		}
	}

	sdCnfFile.end();
}

void SmartClockConfig::saveConfig() {

	// try to rewrite a config file
	if (!sdCnfFile.beginWrite(CNF_FILE_NAME, CNF_MAX_LINE_LENGHT)) {
		LOGGER_PRINT("Failed to write configuration file: ");
		LOGGER_PRINTLN(CNF_FILE_NAME);
		return;
	}

	if(sprintf(buffer, "%d", configFontPreset)>0) {
		sdCnfFile.writeSetting(CNF_KEY_FONTS, buffer);
	}

	sdCnfFile.writeSetting(CNF_KEY_SHOWDATE, cnfShowDate?"Y":"N");

	if(sprintf(buffer, "0x%x", cnfColorTime)>0) {
		sdCnfFile.writeSetting(CNF_KEY_COLOR_TIME, buffer);
	}

	if(sprintf(buffer, "0x%x", cnfColorDate)>0) {
		sdCnfFile.writeSetting(CNF_KEY_COLOR_DATE, buffer);
	}

	if(sprintf(buffer, "0x%x", cnfColorHum)>0) {
		sdCnfFile.writeSetting(CNF_KEY_COLOR_HUM, buffer);
	}

	if(sprintf(buffer, "0x%x", cnfColorTemp)>0) {
		sdCnfFile.writeSetting(CNF_KEY_COLOR_TEMP, buffer);
	}

	sdCnfFile.end();
}
