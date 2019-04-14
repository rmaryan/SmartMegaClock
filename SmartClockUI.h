/*
 * SmartClockUI.h
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

#ifndef SMARTCLOCKUI_H_
#define SMARTCLOCKUI_H_

#include <TFT_HX8357.h>
#include <SD.h>

//Safeguard this include with the IFDEF - that was not done properly inside, and Eclipse/Sloeber crashes because of this
#ifndef _FREESANS24PT7b_
#define _FREESANS24PT7b_
#include <Fonts/GFXFF/FreeSans24pt7b.h>
#endif

// buffer size for the bitmap files reading from the SD
#define BUFF_SIZE 80

/**
 * A smart clock UI class
 */
class SmartClockUI {
private:
	static const uint16_t HOURS_LEFT = 115; // top boundary of the hours text
	static const uint16_t HOURS_TOP = 85; // left boundary of the hours text
	static const uint16_t SECONDS_TOP_SHIFT = 24; // seconds top shift from the hours text
	static const uint16_t ICON_TOP = 210; // top boundary of the icon
	static const uint16_t ICON_LEFT = 10; // left boundary of the icon
	static const uint16_t TEMP_LEFT = 45; // left boundary of the temperature text
	static const uint16_t TEMP_RIGHT = 240; // right boundary of the temperature text
	static const uint16_t HUM_LEFT = 360; // left boundary of the humidity text
	static const uint16_t HUM_TEMP_TOP = 260; // top boundary of the temp/humidity text
	static const uint8_t ICON_DAY_START = 8; // the hour when the day starts
	static const uint8_t ICON_NIGHT_START = 20; // the hour when the night starts

	static const uint16_t BG_COLOR = TFT_BLACK;    // background color
	static const uint16_t TIME_DIMMED_COLOR = 0x39C4; // dimmed time color (for colon)

	static char* TEMP_UOM_LABEL;  // temperature unit of measure label
	static char* HUM_UOM_LABEL;   // humidity unit of measure label
	static char* DAY_ICON_FILE;   // day icon file name on SD
	static char* NIGHT_ICON_FILE; // night icon file name on SD

	static uint8_t omm;
	static uint16_t xcolon;
	static uint16_t xsecs;

	/*
	 * Indicates which icon is being shown on a screen
	 * 0 - unknown
	 * 1 - day
	 * 2 - night
	 */
	static uint8_t partOfDay;

	// Reference to the TFT screen library
	static TFT_HX8357 tft;

	/*
	 * Support functions for drawBMP()
	 * Read 16 and 32-bit types from the SD card file
	 */
	static uint16_t read16(File& f);
	static uint32_t read32(File& f);

	/*
	 * Draw a BMP format bitmap to the screen
	 */
	static void drawBMP(char *filename, int x, int y, boolean flip);

public:
	static void init();
	static void refreshScreen(uint8_t hh, uint8_t mm, uint8_t ss, float hum, float temp);
};

#endif /* SMARTCLOCKUI_H_ */
