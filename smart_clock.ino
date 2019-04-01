/*
 * The the smart clock firmware
 *
 * Used components:
 * - Ardiono MEGA 2560
 * - LCD screen 320x480 HX8357B
 * - Temperature sensor DHT22
 * - Wifi module ES-01 connected to Serial1
 * - RTC module DS3231
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

#include <DHT.h>
#include <DS3231.h>
#include <Wire.h>

#include "SmartClockUI.h"
#include "SDConfigFile.h"

// Initialize DHT sensor
#define DHTPIN 17     // DHT sensor is connected to the pin #17
#define DHTTYPE DHT22   // DHT type is DHT22

DHT dht(DHTPIN, DHTTYPE);
float hum;  //Stores humidity value
float temp; //Stores temperature value


// RTC definitions
#define TIMER_REFRESH_CYCLE 1000

DS3231 clock;
bool h12;
bool PM;
uint8_t hh, mm, ss;

// Clock UI class
SmartClockUI* clockUI = NULL;

// commands processing block
// we should handle both WiFi serial stream and a hardware serial
String inputString = "";      // a String to hold incoming data
String inputStringWiFi = "";  // a String to hold incoming data

unsigned long timeForRefresh = 0; // the counter to detect when it is time to refresh the screen

void loadConfig() {
	//TODO To be completed
	return;
}

void setup() {
	Serial.begin(57600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	// Serial1 is where we connected a WiFi module
	Serial1.begin(57600);

	// reserve 200 bytes for the inputString:
	inputString.reserve(200);

	dht.begin();

	Wire.begin();

	clockUI = new SmartClockUI();

	// load configuration from the SD card
	loadConfig();
}

/*
 * Process the command
 */
void processCommand(const String command, bool isFromWiFi) {
	// store the reference to the command source
	HardwareSerial* serial = isFromWiFi ? &Serial1 : &Serial;

	//TODO Handle commands here

	serial->print("Received command: '");
	serial->print(command);
	serial->println("'");

	//Print temp and humidity values
	serial->print("Humidity: ");
	serial->print(hum);
	serial->print(" %, Temp: ");
	serial->print(temp);
	serial->println(" Celsius");
}

/*
 * SerialEvent occurs whenever a new data comes in the hardware serial RX. This
 * routine is run between each time loop() runs, so using delay inside loop can
 * delay response. Multiple bytes of data may be available.
 * Please note - we are handling two serial streams. One from SerialMonitor
 * and another - from the WiFi board
 */
void serialEvent() {
	while (Serial.available()) {
		// check for string overflow
		if(inputString.length()>150) {
			Serial.println("Command is too long...");
			inputString = "";
		} else {
			// get the new byte
			char inChar = Serial.read();
			// if the incoming character is a newline - process a complete command
			if (inChar == '\n') {
				inputString.trim();
				processCommand(inputString, false);
				inputString = "";
			} else {
				// add it to the inputString:
				inputString += inChar;
			}
		}
	}
}

/*
 * This serial event handler deals with the WiFi serial stream
 */
void serialEvent1() {
	while (Serial1.available()) {
		// check for string overflow
		if(inputStringWiFi.length()>150) {
			Serial1.println("Command is too long...");
			inputStringWiFi = "";
		} else {
			// get the new byte
			int inChar = Serial1.read();
			// process only ASCII chars
			if((inChar>9) && (inChar < 122)) {
				// if the incoming character is a newline - process a complete command
				if (inChar == '\n') {
					inputStringWiFi.trim();
					processCommand(inputStringWiFi, true);
					inputStringWiFi = "";
				} else {
					// add it to the inputString:
					inputStringWiFi += (char)inChar;
				}
			}
		}
	}
}

void loop() {
	if(millis() > timeForRefresh) {
		timeForRefresh = millis()+TIMER_REFRESH_CYCLE;

		// Get the hour, minute, and second
		hh = clock.getHour(h12, PM);
		mm = clock.getMinute();
		ss = clock.getSecond();
		hum = 0;
		temp = 0;
		if (!(ss % 10)) {
			//Read the humidity and temperature values
			hum = dht.readHumidity();
			temp = dht.readTemperature();
		}

		clockUI->refreshScreen(hh, mm, ss, hum, temp);
	}
}
