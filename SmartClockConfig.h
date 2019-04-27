/*
 * SmartClockConfig.h
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

#ifndef SMARTCLOCKCONFIG_H_
#define SMARTCLOCKCONFIG_H_

#include <TFT_HX8357.h>
#include "SDConfigFile.h"

/**
 * A smart clock settings management class
 */
class SmartClockConfig {

private:
	static SDConfigFile sdCnfFile;
	static const uint8_t CNF_MAX_LINE_LENGHT = 40;

	static const char * CNF_FILE_NAME;
	static const char * CNF_KEY_SHOWDATE;
	static const char * CNF_KEY_COLOR_TIME;
	static const char * CNF_KEY_COLOR_DATE;
	static const char * CNF_KEY_COLOR_HUM;
	static const char * CNF_KEY_COLOR_TEMP;

	static const bool     CNF_DEFAULT_SHOW_DATE = false;
	static const uint16_t CNF_DEFAULT_COLOR_TIME = TFT_YELLOW;
	static const uint16_t CNF_DEFAULT_COLOR_DATE = TFT_YELLOW;
	static const uint16_t CNF_DEFAULT_COLOR_HUM = TFT_CYAN;
	static const uint16_t CNF_DEFAULT_COLOR_TEMP = TFT_GREEN;

	static char buffer[10];

public:
	static bool     cnfShowDate;
	static uint16_t cnfColorTime;
	static uint16_t cnfColorDate;
	static uint16_t cnfColorHum;
	static uint16_t cnfColorTemp;

	static void setDefaults();
	static void loadConfig();
	static void saveConfig();
};

#endif /* SMARTCLOCKCONFIG_H_ */
