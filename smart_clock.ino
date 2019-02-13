/*
 * The firmware for the smart clock.
 * Used components:
 * - Ardiono MEGA 2560
 * - LCD screen 320x480 HX8357B
 * - Temperature sensor DHT22
 * - Wifi module ES-01
 * - RTC module DS3231
 */

#include <DHT.h>
#include <DS3231.h>
#include <Wire.h>
#include <TFT_HX8357.h>
#include <SD.h>

//Safeguard this include with the IFDEF - that was not done properly inside, and Eclipse/Sloeber crashes because of this
#ifndef _FREESANS24PT7b_
#define _FREESANS24PT7b_
#include <Fonts/GFXFF/FreeSans24pt7b.h>
#endif

#define DHTPIN 17     // DHT sensor is connected to the pin #17
#define DHTTYPE DHT22   // DHT type is DHT22

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);
float hum;  //Stores humidity value
float temp; //Stores temperature value

// RTC definitions
DS3231 clock;
bool h12;
bool PM;

// LCD screen definitions
TFT_HX8357 tft = TFT_HX8357();
byte omm = 99;
uint8_t hh, mm, ss;
byte xcolon = 0;
int xsecs = 0;

const uint16_t HOURS_LEFT = 90; // top boundary of the hours text
const uint16_t HOURS_TOP = 85; // left boundary of the hours text
const uint16_t SECONDS_TOP_SHIFT = 24; // seconds top shift from the hours text
const uint16_t TEMP_LEFT = 45; // left boundary of the temperature text
const uint16_t TEMP_RIGHT = 240; // right boundary of the temperature text
const uint16_t HUM_LEFT = 360; // left boundary of the humidity text
const uint16_t HUM_TEMP_TOP = 260; // top boundary of the temp/humidity text

const uint16_t BG_COLOR = TFT_BLACK;    // background color
const uint16_t TIME_COLOR = TFT_YELLOW; // time color
const uint16_t TIME_DIMMED_COLOR = 0x39C4; // dimmed time color (for colon)
const uint16_t TEMP_COLOR = TFT_GREEN;     // temperature color
const uint16_t HUM_COLOR = TFT_CYAN;       // humidity color

char TEMP_UOM_LABEL[] = "C"; // temperature unit of measure label
char HUM_UOM_LABEL[] = "%";  // humidity unit of measure label

void setup() {
	// Serial1 is where we connected a WiFi module
	Serial1.begin(9600);

	dht.begin();

	Wire.begin();

	tft.init();
	// we had to patch the original library to make the screen working in the position we need
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
		Serial.println("SD initialization failed!");
		while (1)
			;
	}
}

void loop() {
	// refresh screen every second
	delay(1000);

	// Get the hour, minute, and second
	hh = clock.getHour(h12, PM);
	mm = clock.getMinute();
	ss = clock.getSecond();

	int xpos = HOURS_LEFT;
	int ypos = HOURS_TOP;
	int ysecs = ypos + SECONDS_TOP_SHIFT;

	tft.setTextColor(TIME_COLOR, BG_COLOR);
	if (omm != mm) { // Redraw hours and minutes time every minute
		omm = mm;
		// Draw hours and minutes
		if (hh < 10)
			xpos += tft.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
		xpos += tft.drawNumber(hh, xpos, ypos, 8);    // Draw hours
		xcolon = xpos; // Save colon coord for later to flash on/off later
		xpos += tft.drawChar(':', xpos, ypos - 8, 8);
		if (mm < 10)
			xpos += tft.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
		xpos += tft.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
		xsecs = xpos; // Save seconds 'x' position for later display updates
	}
	xpos = xsecs;

	if (ss % 2) { // Flash the colons on/off
		tft.setTextColor(TIME_DIMMED_COLOR, BG_COLOR);  // Set colour to grey to dim colon
		tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
		xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
		tft.setTextColor(TIME_COLOR, BG_COLOR);    // Set colour back to yellow
	} else {
		tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
		xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
	}

	//Draw seconds
	if (ss < 10)
		xpos += tft.drawChar('0', xpos, ysecs, 6); // Add leading zero

	tft.drawNumber(ss, xpos, ysecs, 6);            // Draw seconds

	// this block is going to be executed every tenth second
	if (!(ss % 10)) {
		//Read the humidity and temperature values
		hum = dht.readHumidity();
		temp = dht.readTemperature();

		int xpos_t = TEMP_LEFT;
		int ypos_t = HUM_TEMP_TOP;
		int xpos_h = HUM_LEFT;

		tft.fillRect(xpos_t, ypos_t, TEMP_RIGHT, tft.height() - ypos_t, BG_COLOR);

		tft.setTextColor(TEMP_COLOR, BG_COLOR);
		xpos_t += tft.drawFloat(temp, 1, xpos_t, ypos_t, 7);
		xpos_t += tft.drawChar('O', xpos_t, ypos_t, 4);
		tft.drawString(TEMP_UOM_LABEL, xpos_t, ypos_t, 1);

		tft.fillRect(xpos_h, ypos_t, tft.width() - xpos_t, tft.height() - ypos_t, BG_COLOR);

		tft.setTextColor(HUM_COLOR, BG_COLOR);
		xpos_h += tft.drawNumber(hum, xpos_h, ypos_t, 7);
		tft.drawString(HUM_UOM_LABEL, xpos_h, ypos_t, 1);

		//Print temp and humidity values to WiFi
		Serial1.print("Humidity: ");
		Serial1.print(hum);
		Serial1.print(" %, Temp: ");
		Serial1.print(temp);
		Serial1.println(" Celsius");

	}
}
