EESchema Schematic File Version 4
LIBS:CommandCenter_KICAD-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
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
L max:MAX7219 U?
U 1 1 5D06920C
P 3060 5060
AR Path="/5CF2E53C/5D06920C" Ref="U?"  Part="1" 
AR Path="/5D06256D/5D06920C" Ref="U?"  Part="1" 
F 0 "U?" H 3060 5897 60  0000 C CNN
F 1 "MAX7219" H 3060 5791 60  0000 C CNN
F 2 "" H 3660 5060 60  0000 C CNN
F 3 "" H 3660 5060 60  0000 C CNN
	1    3060 5060
	1    0    0    -1  
$EndComp
$Comp
L LED:HDSP-4830_2 BAR?
U 1 1 5D06955E
P 5880 1820
F 0 "BAR?" H 5880 2487 50  0000 C CNN
F 1 "HDSP-4830_2" H 5880 2396 50  0000 C CNN
F 2 "Display:HDSP-4830" H 5880 1020 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 3880 2020 50  0001 C CNN
	1    5880 1820
	1    0    0    -1  
$EndComp
$Comp
L LED:HDSP-4830_2 BAR?
U 1 1 5D06B04F
P 5880 3240
F 0 "BAR?" H 5880 3907 50  0000 C CNN
F 1 "HDSP-4830_2" H 5880 3816 50  0000 C CNN
F 2 "Display:HDSP-4830" H 5880 2440 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 3880 3440 50  0001 C CNN
	1    5880 3240
	1    0    0    -1  
$EndComp
$Comp
L LED:HDSP-4830_2 BAR?
U 1 1 5D06C294
P 5880 4670
F 0 "BAR?" H 5880 5337 50  0000 C CNN
F 1 "HDSP-4830_2" H 5880 5246 50  0000 C CNN
F 2 "Display:HDSP-4830" H 5880 3870 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 3880 4870 50  0001 C CNN
	1    5880 4670
	1    0    0    -1  
$EndComp
$Comp
L LED:HDSP-4830_2 BAR?
U 1 1 5D06D339
P 5900 6150
F 0 "BAR?" H 5900 6817 50  0000 C CNN
F 1 "HDSP-4830_2" H 5900 6726 50  0000 C CNN
F 2 "Display:HDSP-4830" H 5900 5350 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-1798EN" H 3900 6350 50  0001 C CNN
	1    5900 6150
	1    0    0    -1  
$EndComp
Text GLabel 2460 4610 0    50   Input ~ 0
DIG0
Text GLabel 2460 4510 0    50   Input ~ 0
BG_MOSI_5V
Text GLabel 2460 4710 0    50   Input ~ 0
DIG4
Text GLabel 2460 4910 0    50   Input ~ 0
DIG6
Text GLabel 2460 5010 0    50   Input ~ 0
DIG2
Text GLabel 2460 5110 0    50   Input ~ 0
DIG3
Text GLabel 2460 5210 0    50   Input ~ 0
DIG7
Text GLabel 2460 5410 0    50   Input ~ 0
DIG5
Text GLabel 2460 5510 0    50   Input ~ 0
DIG1
Text GLabel 2460 5610 0    50   Input ~ 0
BG_SS_5V
Text GLabel 3660 4710 2    50   Output ~ 0
SEG_DP
Text GLabel 3660 5510 2    50   Output ~ 0
SEG_A
Text GLabel 3660 5310 2    50   Output ~ 0
SEG_B
Text GLabel 3660 4910 2    50   Output ~ 0
SEG_C
Text GLabel 3660 4610 2    50   Output ~ 0
SEG_D
$Comp
L power:+5V #PWR?
U 1 1 5D07C652
P 4250 4960
AR Path="/5CF807D7/5D07C652" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D07C652" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4250 4810 50  0001 C CNN
F 1 "+5V" H 4250 5100 50  0000 C CNN
F 2 "" H 4250 4960 50  0001 C CNN
F 3 "" H 4250 4960 50  0001 C CNN
	1    4250 4960
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5D07C658
P 4250 5360
F 0 "C?" H 4275 5460 50  0000 L CNN
F 1 "0.1u" H 4275 5260 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 4288 5210 50  0001 C CNN
F 3 "" H 4250 5360 50  0001 C CNN
	1    4250 5360
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5D07C65E
P 3580 2090
F 0 "C?" H 3605 2190 50  0000 L CNN
F 1 "0.1u" H 3605 1990 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 3618 1940 50  0001 C CNN
F 3 "" H 3580 2090 50  0001 C CNN
	1    3580 2090
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D07C670
P 3580 2270
AR Path="/5CF807D7/5D07C670" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D07C670" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3580 2020 50  0001 C CNN
F 1 "GND" H 3580 2120 50  0000 C CNN
F 2 "" H 3580 2270 50  0001 C CNN
F 3 "" H 3580 2270 50  0001 C CNN
	1    3580 2270
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D07C676
P 4250 5610
AR Path="/5CF807D7/5D07C676" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D07C676" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4250 5360 50  0001 C CNN
F 1 "GND" H 4250 5460 50  0000 C CNN
F 2 "" H 4250 5610 50  0001 C CNN
F 3 "" H 4250 5610 50  0001 C CNN
	1    4250 5610
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 5510 4250 5610
Wire Wire Line
	3580 2240 3580 2270
Wire Wire Line
	4250 4960 4250 5010
Wire Wire Line
	3660 5010 4250 5010
Connection ~ 4250 5010
Wire Wire Line
	4250 5010 4250 5110
$Comp
L Device:R R?
U 1 1 5D07D658
P 4000 5110
F 0 "R?" V 4100 5110 50  0000 C CNN
F 1 "22k" V 4000 5110 50  0000 C CNN
F 2 "" V 3930 5110 50  0001 C CNN
F 3 "~" H 4000 5110 50  0001 C CNN
	1    4000 5110
	0    1    1    0   
$EndComp
Wire Wire Line
	3660 5110 3850 5110
Wire Wire Line
	4150 5110 4250 5110
Connection ~ 4250 5110
Wire Wire Line
	4250 5110 4250 5210
NoConn ~ 3660 4810
NoConn ~ 3660 4510
NoConn ~ 3660 5210
NoConn ~ 3660 5410
Text GLabel 3660 5610 2    50   Input ~ 0
BG_SCK_5V
$Comp
L power:GND #PWR?
U 1 1 5D07EC1F
P 1840 5430
AR Path="/5CF807D7/5D07EC1F" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D07EC1F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1840 5180 50  0001 C CNN
F 1 "GND" H 1840 5280 50  0000 C CNN
F 2 "" H 1840 5430 50  0001 C CNN
F 3 "" H 1840 5430 50  0001 C CNN
	1    1840 5430
	1    0    0    -1  
$EndComp
Wire Wire Line
	2460 4810 1840 4810
Wire Wire Line
	1840 4810 1840 5310
Wire Wire Line
	2460 5310 1840 5310
Connection ~ 1840 5310
Wire Wire Line
	1840 5310 1840 5430
Text GLabel 6100 6650 2    50   Output ~ 0
DIG0
Text GLabel 6100 6550 2    50   Output ~ 0
DIG1
Text GLabel 6100 6450 2    50   Output ~ 0
DIG2
Text GLabel 6100 6350 2    50   Output ~ 0
DIG3
Text GLabel 6100 6250 2    50   Output ~ 0
DIG4
Text GLabel 6100 6150 2    50   Output ~ 0
DIG5
Text GLabel 6100 6050 2    50   Output ~ 0
DIG6
Text GLabel 6100 5950 2    50   Output ~ 0
DIG7
Text GLabel 6100 5850 2    50   Output ~ 0
DIG0
Text GLabel 6100 5750 2    50   Output ~ 0
DIG1
Text GLabel 6080 5170 2    50   Output ~ 0
DIG2
Text GLabel 6080 5070 2    50   Output ~ 0
DIG3
Text GLabel 6080 4970 2    50   Output ~ 0
DIG4
Text GLabel 6080 4870 2    50   Output ~ 0
DIG5
Text GLabel 6080 4770 2    50   Output ~ 0
DIG6
Text GLabel 6080 4670 2    50   Output ~ 0
DIG7
Text GLabel 6080 4570 2    50   Output ~ 0
DIG0
Text GLabel 6080 4470 2    50   Output ~ 0
DIG1
Text GLabel 6080 4370 2    50   Output ~ 0
DIG2
Text GLabel 6080 4270 2    50   Output ~ 0
DIG3
Text GLabel 6080 3740 2    50   Output ~ 0
DIG4
Text GLabel 6080 3640 2    50   Output ~ 0
DIG5
Text GLabel 6080 3540 2    50   Output ~ 0
DIG6
Text GLabel 6080 3440 2    50   Output ~ 0
DIG7
Text GLabel 6080 3340 2    50   Output ~ 0
DIG0
Text GLabel 6080 3240 2    50   Output ~ 0
DIG1
Text GLabel 6080 3140 2    50   Output ~ 0
DIG2
Text GLabel 6080 3040 2    50   Output ~ 0
DIG3
Text GLabel 6080 2940 2    50   Output ~ 0
DIG4
Text GLabel 6080 2840 2    50   Output ~ 0
DIG5
Text GLabel 6080 2320 2    50   Output ~ 0
DIG6
Text GLabel 6080 2220 2    50   Output ~ 0
DIG7
Text GLabel 6080 2120 2    50   Output ~ 0
DIG0
Text GLabel 6080 2020 2    50   Output ~ 0
DIG1
Text GLabel 6080 1920 2    50   Output ~ 0
DIG2
Text GLabel 6080 1820 2    50   Output ~ 0
DIG3
Text GLabel 6080 1720 2    50   Output ~ 0
DIG4
Text GLabel 6080 1620 2    50   Output ~ 0
DIG5
Text GLabel 6080 1520 2    50   Output ~ 0
DIG6
Text GLabel 6080 1420 2    50   Output ~ 0
DIG7
Text GLabel 5560 5950 0    50   Input ~ 0
SEG_DP
Wire Wire Line
	5560 5950 5660 5950
Wire Wire Line
	5660 5950 5660 6050
Wire Wire Line
	5660 6650 5700 6650
Connection ~ 5660 5950
Wire Wire Line
	5660 5950 5700 5950
Wire Wire Line
	5700 6550 5660 6550
Connection ~ 5660 6550
Wire Wire Line
	5660 6550 5660 6650
Wire Wire Line
	5700 6450 5660 6450
Connection ~ 5660 6450
Wire Wire Line
	5660 6450 5660 6550
Wire Wire Line
	5700 6350 5660 6350
Connection ~ 5660 6350
Wire Wire Line
	5660 6350 5660 6450
Wire Wire Line
	5700 6250 5660 6250
Connection ~ 5660 6250
Wire Wire Line
	5660 6250 5660 6350
Wire Wire Line
	5700 6150 5660 6150
Connection ~ 5660 6150
Wire Wire Line
	5660 6150 5660 6250
Wire Wire Line
	5700 6050 5660 6050
Connection ~ 5660 6050
Wire Wire Line
	5660 6050 5660 6150
Text GLabel 5560 4670 0    50   Input ~ 0
SEG_A
Wire Wire Line
	5560 4670 5610 4670
Wire Wire Line
	5610 4670 5610 4770
Wire Wire Line
	5610 5850 5700 5850
Connection ~ 5610 4670
Wire Wire Line
	5610 4670 5680 4670
Wire Wire Line
	5680 5170 5610 5170
Connection ~ 5610 5170
Wire Wire Line
	5610 5170 5610 5750
Wire Wire Line
	5700 5750 5610 5750
Connection ~ 5610 5750
Wire Wire Line
	5610 5750 5610 5850
Wire Wire Line
	5680 5070 5610 5070
Connection ~ 5610 5070
Wire Wire Line
	5610 5070 5610 5170
Wire Wire Line
	5680 4970 5610 4970
Connection ~ 5610 4970
Wire Wire Line
	5610 4970 5610 5070
Wire Wire Line
	5680 4870 5610 4870
Connection ~ 5610 4870
Wire Wire Line
	5610 4870 5610 4970
Wire Wire Line
	5680 4770 5610 4770
Connection ~ 5610 4770
Wire Wire Line
	5610 4770 5610 4870
Text GLabel 5550 3440 0    50   Input ~ 0
SEG_B
Wire Wire Line
	5680 3440 5590 3440
Wire Wire Line
	5590 3440 5590 3540
Wire Wire Line
	5590 4570 5680 4570
Connection ~ 5590 3440
Wire Wire Line
	5590 3440 5550 3440
Wire Wire Line
	5680 4470 5590 4470
Connection ~ 5590 4470
Wire Wire Line
	5590 4470 5590 4570
Wire Wire Line
	5680 4370 5590 4370
Connection ~ 5590 4370
Wire Wire Line
	5590 4370 5590 4470
Wire Wire Line
	5680 4270 5590 4270
Connection ~ 5590 4270
Wire Wire Line
	5590 4270 5590 4370
Wire Wire Line
	5680 3740 5590 3740
Connection ~ 5590 3740
Wire Wire Line
	5590 3740 5590 4270
Wire Wire Line
	5680 3640 5590 3640
Connection ~ 5590 3640
Wire Wire Line
	5590 3640 5590 3740
Wire Wire Line
	5680 3540 5590 3540
Connection ~ 5590 3540
Wire Wire Line
	5590 3540 5590 3640
Text GLabel 5560 2220 0    50   Input ~ 0
SEG_C
Wire Wire Line
	5560 2220 5600 2220
Wire Wire Line
	5600 2220 5600 2320
Wire Wire Line
	5600 3340 5680 3340
Connection ~ 5600 2220
Wire Wire Line
	5600 2220 5680 2220
Wire Wire Line
	5680 3240 5600 3240
Connection ~ 5600 3240
Wire Wire Line
	5600 3240 5600 3340
Wire Wire Line
	5680 3140 5600 3140
Connection ~ 5600 3140
Wire Wire Line
	5600 3140 5600 3240
Wire Wire Line
	5680 3040 5600 3040
Connection ~ 5600 3040
Wire Wire Line
	5600 3040 5600 3140
Wire Wire Line
	5680 2940 5600 2940
Connection ~ 5600 2940
Wire Wire Line
	5600 2940 5600 3040
Wire Wire Line
	5680 2840 5600 2840
Connection ~ 5600 2840
Wire Wire Line
	5600 2840 5600 2940
Wire Wire Line
	5680 2320 5600 2320
Connection ~ 5600 2320
Wire Wire Line
	5600 2320 5600 2840
Text GLabel 5560 1420 0    50   Input ~ 0
SEG_C
Wire Wire Line
	5560 1420 5600 1420
Wire Wire Line
	5600 1420 5600 1520
Wire Wire Line
	5600 2120 5680 2120
Connection ~ 5600 1420
Wire Wire Line
	5600 1420 5680 1420
Wire Wire Line
	5680 2020 5600 2020
Connection ~ 5600 2020
Wire Wire Line
	5600 2020 5600 2120
Wire Wire Line
	5680 1920 5600 1920
Connection ~ 5600 1920
Wire Wire Line
	5600 1920 5600 2020
Wire Wire Line
	5680 1820 5600 1820
Connection ~ 5600 1820
Wire Wire Line
	5600 1820 5600 1920
Wire Wire Line
	5680 1720 5600 1720
Connection ~ 5600 1720
Wire Wire Line
	5600 1720 5600 1820
Wire Wire Line
	5680 1620 5600 1620
Connection ~ 5600 1620
Wire Wire Line
	5600 1620 5600 1720
Wire Wire Line
	5680 1520 5600 1520
Connection ~ 5600 1520
Wire Wire Line
	5600 1520 5600 1620
$Comp
L Logic_LevelTranslator:TXB0104PW U?
U 1 1 5D0B2246
P 3070 2930
F 0 "U?" H 3070 2141 50  0000 C CNN
F 1 "TXB0104PW" H 3070 2050 50  0000 C CNN
F 2 "Package_SO:TSSOP-14_4.4x5mm_P0.65mm" H 3070 2180 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/txb0104.pdf" H 3180 3025 50  0001 C CNN
	1    3070 2930
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5D0B44BD
P 2970 1810
AR Path="/5CF807D7/5D0B44BD" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D0B44BD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2970 1660 50  0001 C CNN
F 1 "+3.3V" H 2970 1950 50  0000 C CNN
F 2 "" H 2970 1810 50  0001 C CNN
F 3 "" H 2970 1810 50  0001 C CNN
	1    2970 1810
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D0B4F8D
P 3170 1810
AR Path="/5CF807D7/5D0B4F8D" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D0B4F8D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3170 1660 50  0001 C CNN
F 1 "+5V" H 3170 1950 50  0000 C CNN
F 2 "" H 3170 1810 50  0001 C CNN
F 3 "" H 3170 1810 50  0001 C CNN
	1    3170 1810
	1    0    0    -1  
$EndComp
Wire Wire Line
	3170 1810 3170 1940
Wire Wire Line
	2970 2230 2970 1830
Wire Wire Line
	3580 1940 3170 1940
Connection ~ 3170 1940
Wire Wire Line
	3170 1940 3170 2230
$Comp
L Device:C C?
U 1 1 5D0D097B
P 2580 1980
F 0 "C?" H 2605 2080 50  0000 L CNN
F 1 "0.1u" H 2605 1880 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 2618 1830 50  0001 C CNN
F 3 "" H 2580 1980 50  0001 C CNN
	1    2580 1980
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D0D0981
P 2580 2160
AR Path="/5CF807D7/5D0D0981" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D0D0981" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2580 1910 50  0001 C CNN
F 1 "GND" H 2580 2010 50  0000 C CNN
F 2 "" H 2580 2160 50  0001 C CNN
F 3 "" H 2580 2160 50  0001 C CNN
	1    2580 2160
	1    0    0    -1  
$EndComp
Wire Wire Line
	2580 2130 2580 2160
Wire Wire Line
	2580 1830 2970 1830
Connection ~ 2970 1830
Wire Wire Line
	2970 1830 2970 1810
Text GLabel 3470 2630 2    50   Output ~ 0
BG_MOSI_5V
Text GLabel 2670 2630 0    50   Input ~ 0
BG_MOSI_
Text GLabel 3470 2830 2    50   Output ~ 0
BG_SCK_5V
Text GLabel 2670 2830 0    50   Input ~ 0
BG_SCK
Text GLabel 3470 3030 2    50   Output ~ 0
BG_SS_5V
Text GLabel 2670 3030 0    50   Input ~ 0
BG_SS
$Comp
L Device:R R?
U 1 1 5D0DDC41
P 1950 2780
F 0 "R?" V 2050 2780 50  0000 C CNN
F 1 "15k" V 1950 2780 50  0000 C CNN
F 2 "" V 1880 2780 50  0001 C CNN
F 3 "~" H 1950 2780 50  0001 C CNN
	1    1950 2780
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5D0DE1EE
P 1950 2230
F 0 "R?" V 2050 2230 50  0000 C CNN
F 1 "10k" V 1950 2230 50  0000 C CNN
F 2 "" V 1880 2230 50  0001 C CNN
F 3 "~" H 1950 2230 50  0001 C CNN
	1    1950 2230
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5D0DE40B
P 1950 1980
AR Path="/5CF807D7/5D0DE40B" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D0DE40B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1950 1830 50  0001 C CNN
F 1 "+5V" H 1950 2120 50  0000 C CNN
F 2 "" H 1950 1980 50  0001 C CNN
F 3 "" H 1950 1980 50  0001 C CNN
	1    1950 1980
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1980 1950 2080
Wire Wire Line
	1950 2380 1950 2430
$Comp
L power:GND #PWR?
U 1 1 5D0E5101
P 1950 3010
AR Path="/5CF807D7/5D0E5101" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D0E5101" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1950 2760 50  0001 C CNN
F 1 "GND" H 1950 2860 50  0000 C CNN
F 2 "" H 1950 3010 50  0001 C CNN
F 3 "" H 1950 3010 50  0001 C CNN
	1    1950 3010
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2930 1950 3010
Wire Wire Line
	2670 2430 1950 2430
Connection ~ 1950 2430
Wire Wire Line
	1950 2430 1950 2630
NoConn ~ 3470 3230
$Comp
L power:GND #PWR?
U 1 1 5D0EFB46
P 2550 3330
AR Path="/5CF807D7/5D0EFB46" Ref="#PWR?"  Part="1" 
AR Path="/5D06256D/5D0EFB46" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2550 3080 50  0001 C CNN
F 1 "GND" H 2550 3180 50  0000 C CNN
F 2 "" H 2550 3330 50  0001 C CNN
F 3 "" H 2550 3330 50  0001 C CNN
	1    2550 3330
	1    0    0    -1  
$EndComp
Wire Wire Line
	2670 3230 2550 3230
Wire Wire Line
	2550 3230 2550 3330
$EndSCHEMATC
