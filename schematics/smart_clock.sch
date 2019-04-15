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
L mrach_custom:Arduino_Mega2560 A1
U 1 1 5CAA5EC6
P 4700 3400
F 0 "A1" H 4700 1019 60  0000 C CNN
F 1 "Arduino_Mega2560" H 4700 913 60  0000 C CNN
F 2 "" H 5400 6150 60  0001 C CNN
F 3 "" H 5400 6150 60  0001 C CNN
	1    4700 3400
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_B J1
U 1 1 5CAAD499
P 1900 5550
F 0 "J1" H 1670 5447 50  0000 R CNN
F 1 "USB_B" H 1670 5538 50  0000 R CNN
F 2 "" H 2050 5500 50  0001 C CNN
F 3 " ~" H 2050 5500 50  0001 C CNN
	1    1900 5550
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5CABDD09
P 2750 5100
F 0 "#PWR01" H 2750 4850 50  0001 C CNN
F 1 "GND" H 2755 4927 50  0000 C CNN
F 2 "" H 2750 5100 50  0001 C CNN
F 3 "" H 2750 5100 50  0001 C CNN
	1    2750 5100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR02
U 1 1 5CABE836
P 2750 5750
F 0 "#PWR02" H 2750 5600 50  0001 C CNN
F 1 "+5V" H 2765 5923 50  0000 C CNN
F 2 "" H 2750 5750 50  0001 C CNN
F 3 "" H 2750 5750 50  0001 C CNN
	1    2750 5750
	1    0    0    -1  
$EndComp
Text Notes 1400 6050 0    50   ~ 0
Power is received from the built-in \nUSB socket of Arduino Mega
Wire Wire Line
	6000 1750 7750 1750
$Comp
L Device:R R1
U 1 1 5CAEB1EE
P 7750 2400
F 0 "R1" H 7820 2446 50  0000 L CNN
F 1 "10k" H 7820 2355 50  0000 L CNN
F 2 "" V 7680 2400 50  0001 C CNN
F 3 "~" H 7750 2400 50  0001 C CNN
	1    7750 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2550 7750 2750
$Comp
L power:GND #PWR04
U 1 1 5CAEBC68
P 7750 2750
F 0 "#PWR04" H 7750 2500 50  0001 C CNN
F 1 "GND" H 7755 2577 50  0000 C CNN
F 2 "" H 7750 2750 50  0001 C CNN
F 3 "" H 7750 2750 50  0001 C CNN
	1    7750 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2250 7750 1750
$Comp
L power:+5V #PWR03
U 1 1 5CAECAB0
P 7750 950
F 0 "#PWR03" H 7750 800 50  0001 C CNN
F 1 "+5V" H 7765 1123 50  0000 C CNN
F 2 "" H 7750 950 50  0001 C CNN
F 3 "" H 7750 950 50  0001 C CNN
	1    7750 950 
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 5CAEEEA5
P 7750 1450
F 0 "SW1" V 7796 1362 50  0000 R CNN
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
L Timer_RTC:DS3231M U2
U 1 1 5CAF1E87
P 1750 3350
F 0 "U2" H 1750 2861 50  0000 C CNN
F 1 "DS3231M" H 1750 2770 50  0000 C CNN
F 2 "Package_SO:SOIC-16W_7.5x10.3mm_P1.27mm" H 1750 2750 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS3231.pdf" H 2020 3400 50  0001 C CNN
	1    1750 3350
	-1   0    0    -1  
$EndComp
Wire Notes Line width 8 style solid rgb(132, 0, 0)
	7450 5300 7450 3550
Text Notes 7600 4450 0    79   ~ 0
TFT Screen 320x480 HX8357B Arduino MEGA Shield
Wire Notes Line width 8 style solid rgb(132, 0, 0)
	7450 3550 10800 3550
Wire Notes Line width 8 style solid rgb(132, 0, 0)
	10800 3550 10800 5300
Wire Notes Line width 8 style solid rgb(132, 0, 0)
	7450 5300 10800 5300
Entry Wire Line
	6550 2450 6650 2550
Entry Wire Line
	6550 2550 6650 2650
Entry Wire Line
	6550 2650 6650 2750
Entry Wire Line
	6550 2750 6650 2850
Entry Wire Line
	6550 2850 6650 2950
Entry Wire Line
	6550 2950 6650 3050
Entry Wire Line
	6550 3050 6650 3150
Entry Wire Line
	6550 3150 6650 3250
Entry Wire Line
	6550 3250 6650 3350
Entry Wire Line
	6550 3350 6650 3450
Entry Wire Line
	6550 3450 6650 3550
Entry Wire Line
	6550 3550 6650 3650
Entry Wire Line
	6550 3650 6650 3750
Entry Wire Line
	6550 3750 6650 3850
Entry Wire Line
	6550 3850 6650 3950
Entry Wire Line
	6550 3950 6650 4050
Entry Wire Line
	6550 4050 6650 4150
Entry Wire Line
	6550 4150 6650 4250
Entry Wire Line
	6550 4250 6650 4350
Entry Wire Line
	6550 4350 6650 4450
Wire Wire Line
	6000 3950 6550 3950
Wire Wire Line
	6000 4050 6550 4050
Wire Wire Line
	6000 4150 6550 4150
Wire Wire Line
	6000 4250 6550 4250
Wire Wire Line
	6000 4350 6550 4350
Wire Wire Line
	6000 3750 6550 3750
Wire Wire Line
	6000 3850 6550 3850
Wire Wire Line
	6000 3650 6550 3650
Wire Wire Line
	6000 3550 6550 3550
Wire Wire Line
	6000 3450 6550 3450
Wire Wire Line
	6000 3350 6550 3350
Wire Wire Line
	6000 3250 6550 3250
Wire Wire Line
	6000 3150 6550 3150
Wire Wire Line
	6000 3050 6550 3050
Wire Wire Line
	6000 2450 6550 2450
Wire Wire Line
	6000 2550 6550 2550
Wire Wire Line
	6000 2650 6550 2650
Wire Wire Line
	6000 2750 6550 2750
Wire Wire Line
	6000 2850 6550 2850
Wire Wire Line
	6000 2950 6550 2950
Entry Wire Line
	6550 4750 6650 4850
Entry Wire Line
	6550 5250 6650 5350
Entry Wire Line
	6550 5350 6650 5450
Entry Wire Line
	6550 5450 6650 5550
Entry Wire Line
	6550 5550 6650 5650
Wire Wire Line
	6000 4750 6550 4750
Wire Wire Line
	6000 5250 6550 5250
Wire Wire Line
	6000 5350 6550 5350
Wire Wire Line
	6000 5450 6550 5450
Wire Wire Line
	6000 5550 6550 5550
Wire Wire Line
	3400 5450 3250 5450
Wire Wire Line
	3250 5450 3250 6150
Wire Wire Line
	3250 6150 6200 6150
Wire Wire Line
	850  5050 850  6300
Wire Wire Line
	850  6300 6300 6300
Wire Wire Line
	850  5050 1900 5050
Wire Wire Line
	3400 5250 3250 5250
Wire Wire Line
	3250 5250 3250 4900
Wire Wire Line
	3250 4900 1000 4900
Wire Wire Line
	1000 4900 1000 2850
Wire Wire Line
	1000 1450 1800 1450
Wire Wire Line
	1800 1450 1800 1700
Wire Wire Line
	850  3750 850  2350
Wire Wire Line
	850  2350 1800 2350
Wire Wire Line
	1800 2350 1800 2300
Wire Wire Line
	1000 2850 1850 2850
Wire Wire Line
	1850 2850 1850 2950
Connection ~ 1000 2850
Wire Wire Line
	1000 2850 1000 1450
Wire Wire Line
	1750 3750 850  3750
Wire Bus Line
	6650 5900 8050 5900
Wire Bus Line
	8050 5900 8050 5300
$Comp
L mrach_custom:DT-06_WiFi_Board U1
U 1 1 5CB8F77B
P 1600 400
F 0 "U1" H 2628 -187 50  0000 L CNN
F 1 "DT-06_WiFi_Board" H 2628 -278 50  0000 L CNN
F 2 "" H 2250 150 50  0001 C CNN
F 3 "" H 2250 150 50  0001 C CNN
	1    1600 400 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 1650 3400 1650
Wire Wire Line
	2950 2000 2950 1650
Wire Wire Line
	2100 2000 2950 2000
Wire Wire Line
	2450 1350 2450 1550
Wire Wire Line
	2450 1550 3400 1550
Wire Wire Line
	2350 1350 2350 1450
Wire Wire Line
	2350 1450 3400 1450
Entry Wire Line
	6550 5650 6650 5750
Entry Wire Line
	6550 5750 6650 5850
Wire Wire Line
	6550 5650 6200 5650
Wire Wire Line
	6200 5650 6200 6150
Wire Wire Line
	6550 5750 6300 5750
Wire Wire Line
	6300 5750 6300 6300
Wire Wire Line
	2250 3150 2850 3150
Wire Wire Line
	2850 3150 2850 2350
Wire Wire Line
	2850 2350 3400 2350
Wire Wire Line
	3400 2250 2950 2250
Wire Wire Line
	2950 2250 2950 3250
Wire Wire Line
	2950 3250 2250 3250
Wire Wire Line
	2750 5050 2750 5100
Connection ~ 2750 5050
Wire Wire Line
	2750 5050 3400 5050
Wire Wire Line
	850  3750 850  5050
Connection ~ 850  3750
Connection ~ 850  5050
Wire Wire Line
	1900 5150 1900 5050
Connection ~ 1900 5050
Wire Wire Line
	1900 5050 2750 5050
Wire Wire Line
	2200 5750 2750 5750
Wire Wire Line
	3100 5750 3100 5550
Wire Wire Line
	3100 5550 3400 5550
Connection ~ 2750 5750
Wire Wire Line
	2750 5750 3100 5750
Wire Wire Line
	850  2350 850  1550
Wire Wire Line
	850  1550 2250 1550
Wire Wire Line
	2250 1550 2250 1350
Connection ~ 850  2350
Wire Wire Line
	1800 1450 2150 1450
Wire Wire Line
	2150 1350 2150 1450
Connection ~ 1800 1450
Wire Bus Line
	6650 2550 6650 5900
$Comp
L Sensor:DHT11 U3
U 1 1 5CAF1154
P 1800 2000
F 0 "U3" H 1556 2046 50  0000 R CNN
F 1 "DHT22" H 1556 1955 50  0000 R CNN
F 2 "Sensor:Aosong_DHT11_5.5x12.0_P2.54mm" H 1800 1600 50  0001 C CNN
F 3 "" H 1950 2250 50  0001 C CNN
	1    1800 2000
	1    0    0    -1  
$EndComp
Text Notes 1900 2350 0    50   ~ 0
Temperature sensor
Text Notes 1500 1000 0    50   ~ 0
WiFi module
Text Notes 2000 3800 0    50   ~ 0
Real Time Clock module
$EndSCHEMATC
