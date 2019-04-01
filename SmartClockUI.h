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

class SmartClockUI {
private:
	const uint16_t HOURS_LEFT = 115; // top boundary of the hours text
	const uint16_t HOURS_TOP = 85; // left boundary of the hours text
	const uint16_t SECONDS_TOP_SHIFT = 24; // seconds top shift from the hours text
	const uint16_t ICON_TOP = 210; // top boundary of the icon
	const uint16_t ICON_LEFT = 10; // left boundary of the icon
	const uint16_t TEMP_LEFT = 45; // left boundary of the temperature text
	const uint16_t TEMP_RIGHT = 240; // right boundary of the temperature text
	const uint16_t HUM_LEFT = 360; // left boundary of the humidity text
	const uint16_t HUM_TEMP_TOP = 260; // top boundary of the temp/humidity text
	const uint8_t ICON_DAY_START = 8; // the hour when the day starts
	const uint8_t ICON_NIGHT_START = 20; // the hour when the night starts

	const uint16_t BG_COLOR = TFT_BLACK;    // background color
	const uint16_t TIME_COLOR = TFT_YELLOW; // time color
	const uint16_t TIME_DIMMED_COLOR = 0x39C4; // dimmed time color (for colon)
	const uint16_t TEMP_COLOR = TFT_GREEN;     // temperature color
	const uint16_t HUM_COLOR = TFT_CYAN;       // humidity color

	char* TEMP_UOM_LABEL = (char *)"C"; // temperature unit of measure label
	char* HUM_UOM_LABEL = (char *)"%";  // humidity unit of measure label
	char* DAY_ICON_FILE = (char *)"sun.bmp"; // day icon file name on SD
	char* NIGHT_ICON_FILE = (char *)"moon.bmp"; // night icon file name on SD

	byte omm = 99;
	byte xcolon = 0;
	int xsecs = 0;

	/*
	 * Indicates which icon is being shown on a screen
	 * 0 - unknown
	 * 1 - day
	 * 2 - night
	 */
	uint8_t partOfDay = 0;

	/*
	 * Support functions for drawBMP()
	 * Read 16 and 32-bit types from the SD card file
	 */
	uint16_t read16(File& f);
	uint32_t read32(File& f);

	/*
	 * Draw a BMP format bitmap to the screen
	 */
	void drawBMP(char *filename, int x, int y, boolean flip);

	// Reference to the TFT screen library
	TFT_HX8357 tft;

public:
	SmartClockUI();
	void refreshScreen(uint8_t hh, uint8_t mm, uint8_t ss, float hum, float temp);
};

#endif /* SMARTCLOCKUI_H_ */
