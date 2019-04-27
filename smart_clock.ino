/*
 * The the smart clock firmware
 *
 * Used components:
 * - Ardiono MEGA 2560
 * - TFT screen 320x480 HX8357B
 * - Temperature sensor DHT22
 * - Wifi module ESP-M2 DT-06 connected to Serial1
 * - RTC module DS3231
 * - Simple toggle switch - connected to pin #7
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

#include <Wire.h>

#include "SmartClockUI.h"
#include "SmartClockConfig.h"
#include "SmartClockCommander.h"
#include "SmartClockSensors.h"

#include "log.h"

#define TIMER_REFRESH_CYCLE 1000
#define SWITCH_PIN 7

unsigned long timeForRefresh = 0; // the counter to detect when it is time to refresh the screen
int lastDimSwitchState = -1;

void setup() {
	Serial.begin(57600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	LOGGER_PRINTLN("Initializing setup...");

	// Serial1 is where we connected a WiFi module
	Serial1.begin(57600);

	SmartClockSensors::init();

	Wire.begin();

	LOGGER_PRINTLN("Initializing ui...");
	SmartClockUI::init();

	LOGGER_PRINTLN("Loading config...");
	SmartClockConfig::loadConfig();

	LOGGER_PRINTLN("Initializing commander...");
	SmartClockCommander::init(&Serial, &Serial1);

	pinMode(SWITCH_PIN, INPUT);
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
		SmartClockCommander::feedCharacter(0);
	}
}
void serialEvent1() {
	while (Serial1.available()) {
		SmartClockCommander::feedCharacter(1);
	}
}

void loop() {
	if(millis() > timeForRefresh) {
		int dimState = digitalRead(SWITCH_PIN);
		if(dimState!=lastDimSwitchState) {
			SmartClockUI::setForceRefresh();
			SmartClockUI::switchDimmed(dimState);
			lastDimSwitchState = dimState;
		}
		timeForRefresh = millis()+TIMER_REFRESH_CYCLE;
		SmartClockUI::refreshScreen();
	}
}
