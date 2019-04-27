/*
 * SmartClockUI.cpp
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

#include "SmartClockUI.h"

#include "SmartClockConfig.h"
#include "SmartClockSensors.h"

#include "log.h"

#define FONT_L1 8
#define FONT_L2 1
#define FONT_SECONDS 6
#define FONT_L3 7
#define FONT_DEGREE 4

char* SmartClockUI::TEMP_UOM_LABEL = (char *)"C"; // temperature unit of measure label
char* SmartClockUI::HUM_UOM_LABEL = (char *)"%";  // humidity unit of measure label
char* SmartClockUI::DAY_ICON_FILE = (char *)"sun.bmp"; // day icon file name on SD
char* SmartClockUI::NIGHT_ICON_FILE = (char *)"moon.bmp"; // night icon file name on SD
uint8_t SmartClockUI::omm;
uint8_t SmartClockUI::oday;
uint16_t SmartClockUI::xcolon;
uint16_t SmartClockUI::xsecs;
uint16_t SmartClockUI::colorTime;
uint16_t SmartClockUI::colorDate;
uint16_t SmartClockUI::colorHum;
uint16_t SmartClockUI::colorTemp;

bool SmartClockUI::forceRefresh = true;
bool SmartClockUI::dimmed = false;
uint8_t SmartClockUI::partOfDay;
TFT_HX8357 SmartClockUI::tft;

void SmartClockUI::init() {
	omm = 99;
	oday = 99;
	xcolon = 0;
	xsecs = 0;
	partOfDay = 0;


	tft = TFT_HX8357();
	tft.init();
	// we had to patch the original library to make the screen working in the position we need (upside down)
	// #define MADCTL_BGR 0x08
	// was changed to
	// #define MADCTL_BGR 0x0B
	// in the file TFT_HX8357.cpp
	tft.setRotation(1);
	tft.fillScreen(BG_COLOR);
	tft.setFreeFont(&FreeSans24pt7b);
	tft.setTextSize(1);

	// Initializing the SD card
	if (!SD.begin()) {
		LOGGER_PRINTLN("SD initialization failed!");
		while (1)
			;
	}
}


uint16_t SmartClockUI::read16(File& f) {
	// BMP data is stored little-endian, Arduino is little-endian too.
	// May need to reverse subscript order if porting elsewhere.

	uint16_t result;
	((uint8_t *)&result)[0] = f.read(); // LSB
	((uint8_t *)&result)[1] = f.read(); // MSB
	return result;
}

uint32_t SmartClockUI::read32(File& f) {
	uint32_t result;
	((uint8_t *)&result)[0] = f.read(); // LSB
	((uint8_t *)&result)[1] = f.read();
	((uint8_t *)&result)[2] = f.read();
	((uint8_t *)&result)[3] = f.read(); // MSB
	return result;
}

void SmartClockUI::drawBMP(char* filename, int x, int y, boolean flip) {
	// This function opens a Windows Bitmap (BMP) file and
	// displays it at the given coordinates.  It's sped up
	// by reading many pixels worth of data at a time
	// (rather than pixel by pixel).  Increasing the buffer
	// size makes loading a little faster but the law of
	// rapidly diminishing speed improvements applies.
	// Suggest 8 minimum and 85 maximum (3 x this value is
	// stored in a byte = 255/3 max!)
	// A value of 8 is only ~20% slower than 24 or 48!
	// Note that 5 x this value of RAM bytes will be needed
	// Increasing beyond 48 gives little benefit.
	// Use integral division of TFT (or typical often used image)
	// width for slightly better speed to avoid short buffer purging

	if ((x >= tft.width()) || (y >= tft.height())) return;
	File     bmpFile;
	int16_t  bmpWidth, bmpHeight;   // Image W+H in pixels
	//uint8_t  bmpDepth;            // Bit depth (must be 24) but we dont use this
	uint32_t bmpImageoffset;        // Start address of image data in file
	uint32_t rowSize;               // Not always = bmpWidth; may have padding
	uint8_t  sdbuffer[3 * BUFF_SIZE];    // SD read pixel buffer (8 bits each R+G+B per pixel)
	uint16_t tftbuffer[BUFF_SIZE];       // TFT pixel out buffer (16-bit per pixel)
	uint8_t  sd_ptr = sizeof(sdbuffer); // sdbuffer pointer (so BUFF_SIZE must be less than 86)
	int16_t  w, h, col;             // to store width, height, row and column
	//uint8_t  r, g, b;   // brg encoding line concatenated for speed so not used
	uint8_t rotation;     // to restore rotation
	uint8_t tft_ptr = 0;  // buffer pointer

	// Check file exists and open it
	Serial.println(filename);
	if (!(bmpFile = SD.open(filename))) {
		Serial.println(F(" File not found"));
		return;
	}

	// Parse BMP header to get the information we need
	if (read16(bmpFile) == 0x4D42) { // BMP file start signature check
		read32(bmpFile);       // Dummy read to throw away and move on
		read32(bmpFile);       // Read & ignore creator bytes
		bmpImageoffset = read32(bmpFile); // Start of image data
		read32(bmpFile);       // Dummy read to throw away and move on
		bmpWidth  = read32(bmpFile);  // Image width
		bmpHeight = read32(bmpFile);  // Image height

		//if (read16(bmpFile) == 1) {
		// Only proceed if we pass a bitmap file check
		// Number of image planes -- must be '1', depth 24 and 0 (uncompressed format)
		if ((read16(bmpFile) == 1) && (read16(bmpFile) == 24) && (read32(bmpFile) == 0)) {
			//goodBmp = true; // Supported BMP format -- proceed!
			// BMP rows are padded (if needed) to 4-byte boundary
			rowSize = (bmpWidth * 3 + 3) & ~3;
			// Crop area to be loaded
			w = bmpWidth;
			h = bmpHeight;

			// We might need to alter rotation to avoid tedious pointer manipulation
			// Save the current value so we can restore it later
			rotation = tft.getRotation();
			// Use TFT SGRAM coord rotation if flip is set for 25% faster rendering
			if (flip) tft.setRotation((rotation + (flip<<2)) % 8); // Value 0-3 mapped to 4-7

			// We might need to flip and calculate new y plot coordinate
			// relative to top left corner as well...
			switch (rotation) {
			case 0:
				if (flip) y = tft.height() - y - h; break;
			case 1:
				y = tft.height() - y - h; break;
				break;
			case 2:
				if (flip) y = tft.height() - y - h; break;
				break;
			case 3:
				y = tft.height() - y - h; break;
				break;
			}

			// Set TFT address window to image bounds
			// Currently, image will not draw or will be corrputed if it does not fit
			// efficient clipping, but I don't need it to be idiot proof ;-)
			tft.setWindow(x, y, x + w - 1, y + h - 1);

			// Finally we are ready to send rows of pixels, writing like this avoids slow 32 bit multiply
			for (uint32_t pos = bmpImageoffset; pos < bmpImageoffset + h * rowSize ; pos += rowSize) {
				// Seek if we need to on boundaries and arrange to dump buffer and start again
				if (bmpFile.position() != pos) {
					bmpFile.seek(pos);
					sd_ptr = sizeof(sdbuffer);
				}

				// Fill the pixel buffer and plot
				for (col = 0; col < w; col++) { // For each column...
					// Time to read more pixel data?
					if (sd_ptr >= sizeof(sdbuffer)) {
						// Push tft buffer to the display
						if (tft_ptr) {
							// Here we are sending a uint16_t array to the function
							tft.pushColors(tftbuffer, tft_ptr);
							tft_ptr = 0; // tft_ptr and sd_ptr are not always in sync...
						}
						// Finally reading bytes from SD Card
						bmpFile.read(sdbuffer, sizeof(sdbuffer));
						sd_ptr = 0; // Set buffer index to start
					}
					// Convert pixel from BMP 8+8+8 format to TFT compatible 16 bit word
					// Blue 5 bits, green 6 bits and red 5 bits (16 bits total)
					// Is is a long line but it is faster than calling a library fn for this
					tftbuffer[tft_ptr] = (sdbuffer[sd_ptr++] >> 3) ;
					tftbuffer[tft_ptr] |= ((sdbuffer[sd_ptr++] & 0xFC) << 3);
					tftbuffer[tft_ptr] |= ((sdbuffer[sd_ptr++] & 0xF8) << 8);
					tft_ptr++;
				} // Next row
			}   // All rows done

			// Write any partially full buffer to TFT
			if (tft_ptr) tft.pushColors(tftbuffer, tft_ptr);

			tft.setRotation(rotation); // Put back original rotation
		} // End of bitmap access
	}   // End of bitmap file check

	bmpFile.close();
}

void SmartClockUI::setForceRefresh() {
	forceRefresh = true;
}

void SmartClockUI::switchDimmed(int dimmedState) {
	if(dimmedState == HIGH) {
		// set the dimmed colors
		SmartClockUI::colorTime = 0xC618;
		SmartClockUI::colorDate = 0xC618;
		SmartClockUI::colorHum = 0xC618;
		SmartClockUI::colorTemp = 0xC618;
	} else {
		// set the normal colors
		SmartClockUI::colorTime = SmartClockConfig::cnfColorTime;
		SmartClockUI::colorDate = SmartClockConfig::cnfColorDate;
		SmartClockUI::colorHum = SmartClockConfig::cnfColorHum;
		SmartClockUI::colorTemp = SmartClockConfig::cnfColorTemp;
	}
}

void SmartClockUI::refreshScreen() {
	bool h12;
	bool PM;
	uint8_t hh, mm, ss, day, month, year;
	float hum;  //Stores humidity value
	float temp; //Stores temperature value

	// Get the hour, minute, and second
	hh = SmartClockSensors::clock.getHour(h12, PM);
	mm = SmartClockSensors::clock.getMinute();
	ss = SmartClockSensors::clock.getSecond();
	hum = 0;
	temp = 0;
	day=0;
	month=0;
	year=0;

	if(forceRefresh) {
		tft.fillScreen(BG_COLOR);
	}

	if (!(ss % 10) || forceRefresh) {
		//Read the humidity and temperature values
		hum = SmartClockSensors::dht.readHumidity();
		temp = SmartClockSensors::dht.readTemperature();
	}

	// check the date
	if(SmartClockConfig::cnfShowDate) {
		day = SmartClockSensors::clock.getDate();
		if((oday!= day) || forceRefresh) {
			bool Century = false;
			month = SmartClockSensors::clock.getMonth(Century);
			year = SmartClockSensors::clock.getYear();
		}
	}


	uint16_t xpos = HOURS_LEFT;
	uint16_t ypos = SmartClockConfig::cnfShowDate?HOURS_WITH_DATE_TOP:HOURS_TOP;
	int ysecs = ypos + SECONDS_TOP_SHIFT;

	tft.setTextColor(colorTime, BG_COLOR);
	//	tft.setTextSize(1);
	if ((omm != mm) || forceRefresh) { // Redraw hours and minutes time every minute
		omm = mm;
		// Draw hours and minutes
		if (hh < 10)
			xpos += tft.drawChar('0', xpos, ypos, FONT_L1); // Add hours leading zero for 24 hr clock
		xpos += tft.drawNumber(hh, xpos, ypos, FONT_L1);    // Draw hours
		xcolon = xpos; // Save colon coord for later to flash on/off later
		xpos += tft.drawChar(':', xpos, ypos - 8, FONT_L1);
		if (mm < 10)
			xpos += tft.drawChar('0', xpos, ypos, FONT_L1); // Add minutes leading zero
		xpos += tft.drawNumber(mm, xpos, ypos, FONT_L1);    // Draw minutes
		xsecs = xpos; // Save seconds 'x' position for later display updates
	}
	xpos = xsecs;

	if (ss % 2) { // Flash the colons on/off
		tft.setTextColor(TIME_DIMMED_COLOR, BG_COLOR);  // Set colour to grey to dim colon
		tft.drawChar(':', xcolon, ypos - 8, FONT_L1);     // Hour:minute colon
		xpos += tft.drawChar(':', xsecs, ysecs, FONT_SECONDS); // Seconds colon
		tft.setTextColor(colorTime, BG_COLOR);    // Set colour back to yellow
	} else {
		tft.drawChar(':', xcolon, ypos - 8, FONT_L1);     // Hour:minute colon
		xpos += tft.drawChar(':', xsecs, ysecs, FONT_SECONDS); // Seconds colon
	}

	//Draw seconds
	if (ss < 10)
		xpos += tft.drawChar('0', xpos, ysecs, FONT_SECONDS); // Add leading zero

	tft.drawNumber(ss, xpos, ysecs, FONT_SECONDS);            // Draw seconds

	// this block is going to be executed every tenth second or when forced
	if (!(ss % 10) || forceRefresh) {
		//Read the humidity and temperature values
		int xpos_t = TEMP_LEFT;
		int ypos_t = HUM_TEMP_TOP;
		int xpos_h = HUM_LEFT;

		tft.fillRect(xpos_t, ypos_t, TEMP_RIGHT, tft.height() - ypos_t, BG_COLOR);

		tft.setTextColor(colorTemp, BG_COLOR);
		xpos_t += tft.drawFloat(temp, 1, xpos_t, ypos_t, FONT_L3);
		xpos_t += tft.drawChar('O', xpos_t, ypos_t, FONT_DEGREE);
		tft.drawString(TEMP_UOM_LABEL, xpos_t, ypos_t, FONT_L2);

		tft.fillRect(xpos_h, ypos_t, tft.width() - xpos_t, tft.height() - ypos_t, BG_COLOR);

		tft.setTextColor(colorHum, BG_COLOR);
		xpos_h += tft.drawNumber(hum, xpos_h, ypos_t, FONT_L3);
		tft.drawString(HUM_UOM_LABEL, xpos_h, ypos_t, FONT_L2);

		if ((hh>=ICON_DAY_START) && (hh<ICON_NIGHT_START)) {
			if((partOfDay != 1) || forceRefresh) {
				drawBMP(DAY_ICON_FILE, ICON_LEFT, ICON_TOP, 1);
				partOfDay = 1;
			}
		} else {
			if((partOfDay != 2) || forceRefresh) {
				drawBMP(NIGHT_ICON_FILE, ICON_LEFT,ICON_TOP, 1);
				partOfDay = 2;
			}
		}
	}

	// draw date if needed
	if(SmartClockConfig::cnfShowDate) {
		if((oday!= day) || forceRefresh) {
			char dateString[11];
			sprintf(dateString, "%02d/%02d/20%02d", day, month, year);
			tft.setTextColor(colorDate, BG_COLOR);
			tft.drawString(dateString, DATE_LEFT, DATE_TOP, FONT_L2);
			oday = day;
		}
	}
	forceRefresh = false;
}
