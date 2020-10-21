EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L co2-detector-rescue:LED-meteo-home-rescue D1
U 1 1 5F7E20CE
P 3500 1800
F 0 "D1" H 3500 1900 50  0000 C CNN
F 1 "Red" H 3500 1700 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm" H 3500 1800 50  0001 C CNN
F 3 "" H 3500 1800 50  0001 C CNN
	1    3500 1800
	0    -1   -1   0   
$EndComp
$Comp
L co2-detector-rescue:R-meteo-home-rescue R3
U 1 1 5F7E33D8
P 4000 2200
F 0 "R3" V 4080 2200 50  0000 C CNN
F 1 "90" V 4000 2200 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3930 2200 50  0001 C CNN
F 3 "" H 4000 2200 50  0001 C CNN
	1    4000 2200
	1    0    0    -1  
$EndComp
NoConn ~ 4900 2700
NoConn ~ 4900 2800
NoConn ~ 4900 3100
NoConn ~ 4900 3200
NoConn ~ 5600 3650
NoConn ~ 3900 2900
NoConn ~ 3900 2800
NoConn ~ 3900 2700
Wire Wire Line
	3300 3400 3300 2450
Connection ~ 5400 3300
Wire Wire Line
	3900 3100 3700 3100
Wire Wire Line
	3700 3100 3700 3800
Wire Wire Line
	3700 3800 5050 3800
Wire Wire Line
	3900 3000 3750 3000
Wire Wire Line
	3750 3000 3750 2550
Wire Wire Line
	3750 2550 4000 2550
Wire Wire Line
	4000 2550 4000 2350
Wire Wire Line
	3900 3200 3600 3200
Wire Wire Line
	3900 3300 3500 3300
Wire Wire Line
	4000 2050 4000 1950
Wire Wire Line
	6800 3300 6800 2000
Wire Wire Line
	3500 1450 3500 1650
Wire Wire Line
	3750 1650 3750 1450
Connection ~ 3750 1450
Wire Wire Line
	5400 3300 6800 3300
Wire Wire Line
	5400 3300 5400 3750
Wire Wire Line
	3750 1450 3500 1450
Wire Wire Line
	5250 3450 5250 3400
Wire Wire Line
	5250 3400 5050 3400
Wire Wire Line
	5250 3800 5250 3550
Wire Wire Line
	5050 3450 5050 3400
Connection ~ 5050 3400
Wire Wire Line
	5050 3750 5050 3800
Connection ~ 5050 3800
Wire Wire Line
	5050 3800 5250 3800
$Comp
L co2-detector-rescue:R-meteo-home-rescue R4
U 1 1 5F7E3096
P 5050 3600
F 0 "R4" H 5130 3600 50  0000 C CNN
F 1 "10K" H 4950 3600 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4980 3600 50  0001 C CNN
F 3 "" H 5050 3600 50  0001 C CNN
	1    5050 3600
	1    0    0    -1  
$EndComp
$Comp
L co2-detector-rescue:DHT22_Temperature_Humidity-sensors-meteo-home-rescue TH1
U 1 1 5F79E2BA
P 5600 3600
F 0 "TH1" H 5600 4650 60  0000 C CNN
F 1 "DHT22_Temperature_Humidity" H 5600 4550 60  0000 C CNN
F 2 "Sensors:DHT22_Temperature_Humidity" H 5600 3600 60  0001 C CNN
F 3 "" H 5600 3600 60  0000 C CNN
	1    5600 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 3750 5600 3750
Wire Wire Line
	5250 3550 5600 3550
Wire Wire Line
	5600 3450 5250 3450
$Comp
L co2-detector-rescue:SGP30-meteo-home-rescue U2
U 1 1 5F84C63D
P 6100 2750
F 0 "U2" V 6042 2878 60  0000 L CNN
F 1 "SGP30" V 6148 2878 60  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 6100 2750 60  0001 C CNN
F 3 "" H 6100 2750 60  0001 C CNN
	1    6100 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	5300 2900 5300 2800
Wire Wire Line
	5300 2800 5600 2800
Wire Wire Line
	5600 2900 5400 2900
Wire Wire Line
	5400 2900 5400 3000
Wire Wire Line
	3300 2450 5600 2450
Wire Wire Line
	5600 2450 5600 2600
Wire Wire Line
	5600 2700 5400 2700
Wire Wire Line
	5400 2700 5400 2000
Wire Wire Line
	5400 2000 6800 2000
Wire Wire Line
	5400 1450 5400 2000
Connection ~ 5400 2000
$Comp
L co2-detector-rescue:LED-meteo-home-rescue D2
U 1 1 5F7E21B5
P 3750 1800
F 0 "D2" H 3750 1900 50  0000 C CNN
F 1 "Yellow" H 3750 1700 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm" H 3750 1800 50  0001 C CNN
F 3 "" H 3750 1800 50  0001 C CNN
	1    3750 1800
	0    -1   -1   0   
$EndComp
Connection ~ 4000 1450
Wire Wire Line
	4000 1450 5400 1450
Wire Wire Line
	4000 1450 3750 1450
Wire Wire Line
	4000 1650 4000 1450
$Comp
L co2-detector-rescue:LED-meteo-home-rescue D3
U 1 1 5F7E21E7
P 4000 1800
F 0 "D3" H 4000 1900 50  0000 C CNN
F 1 "Green" H 4000 1700 50  0000 C CNN
F 2 "LEDs:LED_D5.0mm" H 4000 1800 50  0001 C CNN
F 3 "" H 4000 1800 50  0001 C CNN
	1    4000 1800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3600 2350 3600 3200
Wire Wire Line
	3750 2050 3750 1950
$Comp
L co2-detector-rescue:R-meteo-home-rescue R1
U 1 1 5F7E333C
P 3500 2200
F 0 "R1" V 3580 2200 50  0000 C CNN
F 1 "160" V 3500 2200 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3430 2200 50  0001 C CNN
F 3 "" H 3500 2200 50  0001 C CNN
	1    3500 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3300 3500 2350
Wire Wire Line
	3500 2050 3500 1950
$Comp
L co2-detector-rescue:R-meteo-home-rescue R2
U 1 1 5F822937
P 3750 2200
F 0 "R2" V 3830 2200 50  0000 C CNN
F 1 "160" V 3750 2200 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3680 2200 50  0001 C CNN
F 3 "" H 3750 2200 50  0001 C CNN
	1    3750 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2350 3750 2350
Wire Wire Line
	5050 3400 4900 3400
Wire Wire Line
	4900 3300 5400 3300
Wire Wire Line
	5400 3000 4900 3000
Wire Wire Line
	4900 2900 5300 2900
$Comp
L co2-detector-rescue:WeMos_D1_mini-wemos_mini-meteo-home-rescue U1
U 1 1 5F79E0BA
P 4400 3050
F 0 "U1" H 4400 3550 60  0000 C CNN
F 1 "WeMos_D1_mini" H 4400 2550 60  0000 C CNN
F 2 "wemos-d1-mini:wemos-d1-mini-with-pin-header" H 4950 2350 60  0001 C CNN
F 3 "" H 4950 2350 60  0000 C CNN
	1    4400 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3400 3900 3400
Text Label 3300 2850 0    50   ~ 0
3.3V
Text Label 5500 2000 0    50   ~ 0
GND
Text Label 5150 3400 0    50   ~ 0
5V
$EndSCHEMATC
