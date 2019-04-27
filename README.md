# SmartMegaClock

## Summary

The firmware for the smart clock.

Components:
 * Ardiono MEGA 2560
 * LCD screen 320x480 HX8357B
 * Temperature sensor DHT22
 * Wifi module ES-01
 * RTC module DS3231
 * Steel Case from the old car inverter

## Functions

The clock shows current time, date, humidity and room temperature.

The clock accepts telnet connections and can be managed using simple command line interface.

All clock settings are stored on the SD card and are preserved if the clock was turned off.

The clock screen can be dimmed using a manual switch. 

The clock can be powered by any USB power source.

![Look and feel](/schematics/smart_clock.png)

## Schematics

![Smart Clock Schematics](/schematics/smart_clock_schematics.png)

## Initial Setup

Place the sdcard folder content to the root of the SD card.

Upload the firmware to Arduino MEGA.

Configure the WiFi module:
 * On the initial boot, WiFi module starts own WiFi network (in our case - "Doit_WiFi_xxxxxx"). Connect to this network and access the configuration page
with a web browser (in our case 192.168.4.1). 

 * Set up the network according to your module documentation. To employ the CLI functionality, switch to the TCP server mode. Thus you can use telnet client to connect to the clock.

 * Make sure to set the serial port speed to 57600 baud to match the speed in the clock source code.

## CLI Commands <implementation pending>

`time [HH:MM]` - set the clock time (in 24-hours format). If no parameter specified - current time is shown. 
Example: `time 12:31`

`date [DD/MM/YYYY]` - set the clock date. If no parameter specified - current date is shown. 
Example: `date 21/12/2019`

`showdate [Y | N]` - turns the date indication on or off. If no parameter specified - current state is shown.
Example: `showdate Y`

`color [time | date | hum | temp  0x123456]` - set the RGB color for the area specified. Please note, RGB colors are converted to 16-bit RGB565 format (since our screen works with the 16-bit colors only). If no parameter specified - current colors are listed.  
Example" `color date 0xFF5a20`

`reset` - reset colors and showdate flag to the factory default state. The configuration file is wiped.

`help` - show the help information about the supported commands.

## Possible Future Features

1. Add light sensor and dim the screen automatically when it gets dark.
2. Add buzzer and implement wake up alarm.
3. Connect clock to some IoT platform and feed the temperature and humidity data to some dashboard.

## Used Libraries

### TFT_HX8357 for the LCD Screen

This is an adoption of the Adafruit GFX library to work with the inexpensive HX8357-based LCD screens.

We also had to patch the original library to allow the screen working upside down (that was the only way to fit the screen into the case we've got).

In file *TFT_HX8357.cpp*:
 
`#define MADCTL_BGR 0x08`

was replaces with

`#define MADCTL_BGR 0x0B`

Library page: https://github.com/Bodmer/TFT_HX8357

### SDConfigFile

SDConfigFile is an Arduino library to read Sketch settings from a configuration file on an SD card.

The original library was updated to support also writing to the config file.

SDConfigFile library is distributed under the terms of LGPL.

Library page: https://github.com/bneedhamia/sdconfigfile

## License 

Copyright (c) 2019 Maryan Rachynskyy
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.