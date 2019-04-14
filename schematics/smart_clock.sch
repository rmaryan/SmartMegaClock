EESchema Schematic File Version 4
LIBS:smart_clock-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "SmartMegaClock"
Date "2019-04-05"
Rev "1.0"
Comp "Mar'yan Rachynskyy"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L mrach_custom:Arduino_Mega2560 A?
U 1 1 5CAA5EC6
P 4700 3400
F 0 "A?" H 4700 1019 60  0000 C CNN
F 1 "Arduino_Mega2560" H 4700 913 60  0000 C CNN
F 2 "" H 5400 6150 60  0001 C CNN
F 3 "" H 5400 6150 60  0001 C CNN
	1    4700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 5550 1450 5550
$Comp
L Connector:USB_B J?
U 1 1 5CAAD499
P 1000 5350
F 0 "J?" H 770 5247 50  0000 R CNN
F 1 "USB_B" H 770 5338 50  0000 R CNN
F 2 "" H 1150 5300 50  0001 C CNN
F 3 " ~" H 1150 5300 50  0001 C CNN
	1    1000 5350
	1    0    0    1   
$EndComp
Wire Wire Line
	1450 5050 1450 4950
Wire Wire Line
	1450 4950 1000 4950
$Comp
L power:GND #PWR?
U 1 1 5CABDD09
P 1450 5050
F 0 "#PWR?" H 1450 4800 50  0001 C CNN
F 1 "GND" H 1455 4877 50  0000 C CNN
F 2 "" H 1450 5050 50  0001 C CNN
F 3 "" H 1450 5050 50  0001 C CNN
	1    1450 5050
	1    0    0    -1  
$EndComp
Connection ~ 1450 5050
$Comp
L power:+5V #PWR?
U 1 1 5CABE836
P 1450 5550
F 0 "#PWR?" H 1450 5400 50  0001 C CNN
F 1 "+5V" H 1465 5723 50  0000 C CNN
F 2 "" H 1450 5550 50  0001 C CNN
F 3 "" H 1450 5550 50  0001 C CNN
	1    1450 5550
	1    0    0    -1  
$EndComp
Connection ~ 1450 5550
Wire Wire Line
	1450 5550 3400 5550
Text Notes 600  5850 0    50   ~ 0
Power is received from the built-in \nUSB socket of Arduino Mega
Wire Wire Line
	6000 1750 7750 1750
Wire Wire Line
	1450 5050 3400 5050
$Comp
L Device:R R?
U 1 1 5CAEB1EE
P 7750 2400
F 0 "R?" H 7820 2446 50  0000 L CNN
F 1 "10k" H 7820 2355 50  0000 L CNN
F 2 "" V 7680 2400 50  0001 C CNN
F 3 "~" H 7750 2400 50  0001 C CNN
	1    7750 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2550 7750 2750
$Comp
L power:GND #PWR?
U 1 1 5CAEBC68
P 7750 2750
F 0 "#PWR?" H 7750 2500 50  0001 C CNN
F 1 "GND" H 7755 2577 50  0000 C CNN
F 2 "" H 7750 2750 50  0001 C CNN
F 3 "" H 7750 2750 50  0001 C CNN
	1    7750 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2250 7750 1750
$Comp
L power:+5V #PWR?
U 1 1 5CAECAB0
P 7750 950
F 0 "#PWR?" H 7750 800 50  0001 C CNN
F 1 "+5V" H 7765 1123 50  0000 C CNN
F 2 "" H 7750 950 50  0001 C CNN
F 3 "" H 7750 950 50  0001 C CNN
	1    7750 950 
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW?
U 1 1 5CAEEEA5
P 7750 1450
F 0 "SW?" V 7796 1362 50  0000 R CNN
F 1 "Dimming Switch" V 7705 1362 50  0000 R CNN
F 2 "" H 7750 1450 50  0001 C CNN
F 3 "~" H 7750 1450 50  0001 C CNN
	1    7750 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7750 1650 7750 1750
Connection ~ 7750 1750
Wire Wire Line
	7750 1250 7750 950 
$Comp
L Sensor:DHT11 U?
U 1 1 5CAF1154
P 1800 1300
F 0 "U?" H 1556 1346 50  0000 R CNN
F 1 "DHT22" H 1556 1255 50  0000 R CNN
F 2 "Sensor:Aosong_DHT11_5.5x12.0_P2.54mm" H 1800 900 50  0001 C CNN
F 3 "" H 1950 1550 50  0001 C CNN
	1    1800 1300
	1    0    0    -1  
$EndComp
$Comp
L Timer_RTC:DS3231M U?
U 1 1 5CAF1E87
P 1750 2350
F 0 "U?" H 1750 1861 50  0000 C CNN
F 1 "DS3231M" H 1750 1770 50  0000 C CNN
F 2 "Package_SO:SOIC-16W_7.5x10.3mm_P1.27mm" H 1750 1750 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS3231.pdf" H 2020 2400 50  0001 C CNN
	1    1750 2350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2100 1300 2800 1300
Wire Wire Line
	2800 1300 2800 1650
Wire Wire Line
	2800 1650 3400 1650
Wire Wire Line
	2250 2250 3400 2250
Wire Wire Line
	2250 2150 2800 2150
Wire Wire Line
	2800 2150 2800 2350
Wire Wire Line
	2800 2350 3400 2350
$EndSCHEMATC
