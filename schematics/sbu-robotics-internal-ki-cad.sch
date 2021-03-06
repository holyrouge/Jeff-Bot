EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:SB_Robotics
LIBS:freetronics_schematic
LIBS:Arduino_As_Uno-cache
LIBS:_electromech
LIBS:_linear
LIBS:_logic
LIBS:_passive
LIBS:_semi
LIBS:74hct125d
LIBS:74hct245
LIBS:74xgxx
LIBS:75176
LIBS:Abracon
LIBS:ac-dc
LIBS:acorn_electron_expansion_connector
LIBS:actel
LIBS:ActiveSemi
LIBS:allegro
LIBS:Altera
LIBS:AMS
LIBS:analog_devices
LIBS:AnalogDevices
LIBS:AOS
LIBS:arm-swd-header
LIBS:battery_management
LIBS:bbd
LIBS:bosch
LIBS:brooktre
LIBS:cmos_ieee
LIBS:conn-2mm
LIBS:conn-100mil
LIBS:conn-amphenol
LIBS:conn-assmann
LIBS:conn-cui
LIBS:conn-fci
LIBS:conn-jae
LIBS:conn-linx
LIBS:conn-molex
LIBS:conn-special-headers
LIBS:conn-tagconnect
LIBS:conn-te
LIBS:conn-test
LIBS:dc-dc
LIBS:diode
LIBS:DiodesInc
LIBS:electomech-misc
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:esp8266-esp-01
LIBS:esp8266-esp-03
LIBS:esp8266-esp-12e
LIBS:Fairchild
LIBS:ftdi
LIBS:gennum
LIBS:graphic_symbols
LIBS:hc11
LIBS:hm-11
LIBS:infineon
LIBS:intersil
LIBS:ir
LIBS:iso15
LIBS:Lattice
LIBS:leds
LIBS:LEM
LIBS:LinearTech
LIBS:Littelfuse
LIBS:logic-4000
LIBS:logic-7400
LIBS:logic-7400-new
LIBS:lpc11u14fbd48
LIBS:MACOM
LIBS:Macrofab
LIBS:maxim
LIBS:mcp1700t-3302e-tt
LIBS:mcp73831t-2aci-ot
LIBS:mechanical
LIBS:micro_usb_socket
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic24mcu
LIBS:microchip_pic32mcu
LIBS:Micron
LIBS:mke02z64vld2
LIBS:mke04z8vtg4
LIBS:modules
LIBS:motor_drivers
LIBS:msp430
LIBS:Murata
LIBS:nordicsemi
LIBS:nRF24L01+
LIBS:nrf24l01p_smd
LIBS:nrf51822-04
LIBS:nxp
LIBS:nxp_armmcu
LIBS:OceanOptics
LIBS:onsemi
LIBS:Oscillators
LIBS:pasv-BelFuse
LIBS:pasv-BiTech
LIBS:pasv-Bourns
LIBS:pasv-cap
LIBS:pasv-ind
LIBS:pasv-Murata
LIBS:pasv-res
LIBS:pasv-TDK
LIBS:pasv-xtal
LIBS:pcb
LIBS:Power_Management
LIBS:powerint
LIBS:pp_ws2812b
LIBS:pspice
LIBS:Recom
LIBS:recom-r1se
LIBS:references
LIBS:rfcom
LIBS:RFSolutions
LIBS:Richtek
LIBS:semi-diode-DiodesInc
LIBS:semi-diode-generic
LIBS:semi-diode-MCC
LIBS:semi-diode-NXP
LIBS:semi-diode-OnSemi
LIBS:semi-diode-Semtech
LIBS:semi-diode-ST
LIBS:semi-diode-Toshiba
LIBS:semi-opto-generic
LIBS:semi-opto-liteon
LIBS:semi-thyristor-generic
LIBS:semi-trans-AOS
LIBS:semi-trans-DiodesInc
LIBS:semi-trans-EPC
LIBS:semi-trans-Fairchild
LIBS:semi-trans-generic
LIBS:semi-trans-Infineon
LIBS:semi-trans-IRF
LIBS:semi-trans-IXYS
LIBS:semi-trans-NXP
LIBS:semi-trans-OnSemi
LIBS:semi-trans-Panasonic
LIBS:semi-trans-ST
LIBS:semi-trans-TI
LIBS:semi-trans-Toshiba
LIBS:semi-trans-Vishay
LIBS:sensors
LIBS:silabs
LIBS:skyworks
LIBS:SN754410One
LIBS:ST
LIBS:st_ic
LIBS:stm8
LIBS:stm32
LIBS:stm32f37xxx_48pin
LIBS:stm32f102xx_48pin
LIBS:stm32f103c8t6-module-china
LIBS:stm32f302xx_48pin
LIBS:supertex
LIBS:symbol
LIBS:TexasInstruments
LIBS:transf
LIBS:triac_thyristor
LIBS:ttl_ieee
LIBS:uart_pp
LIBS:u-blox
LIBS:usb_plug
LIBS:video
LIBS:Vishay
LIBS:Winbond
LIBS:wiznet
LIBS:Worldsemi
LIBS:Xicor
LIBS:zetex
LIBS:Zilog
LIBS:sbu-robotics-internal-ki-cad-cache
EELAYER 25 0
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
L SN754410One H1
U 1 1 59DFF376
P 5530 3400
F 0 "H1" H 5530 3300 50  0000 C CNN
F 1 "SN754410One" H 5530 3500 50  0000 C CNN
F 2 "MODULE" H 5530 3400 50  0001 C CNN
F 3 "DOCUMENTATION" H 5530 3400 50  0001 C CNN
	1    5530 3400
	1    0    0    -1  
$EndComp
$Comp
L Motor_DC M1
U 1 1 59DFF4C2
P 5070 4400
F 0 "M1" V 5350 4280 50  0000 L CNN
F 1 "Motor_DC" V 5400 4170 50  0000 L TNN
F 2 "" H 5070 4310 50  0001 C CNN
F 3 "" H 5070 4310 50  0001 C CNN
	1    5070 4400
	0    1    1    0   
$EndComp
$Comp
L Motor_DC M2
U 1 1 59DFF511
P 5930 4400
F 0 "M2" V 6210 4310 50  0000 L CNN
F 1 "Motor_DC" V 6250 4200 50  0000 L TNN
F 2 "" H 5930 4310 50  0001 C CNN
F 3 "" H 5930 4310 50  0001 C CNN
	1    5930 4400
	0    1    1    0   
$EndComp
$Comp
L Arduino_UNO_R3 A1
U 1 1 59E01BD5
P 2760 3350
F 0 "A1" H 2290 4590 50  0000 R CNN
F 1 "Arduino UNO R3" H 2560 4460 50  0000 R CNN
F 2 "Modules:Arduino_UNO_R3" H 2910 2300 50  0001 L CNN
F 3 "" H 2560 4400 50  0001 C CNN
	1    2760 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5280 4400 5270 4400
Text Notes 7340 7600 0    60   ~ 12
SB Robot Design Team Internal Competition 2017 Circuit Design\n\n
$Comp
L +9V #PWR2
U 1 1 59E05DE5
P 4060 4490
F 0 "#PWR2" H 4060 4340 50  0001 C CNN
F 1 "+9V" H 4060 4650 50  0000 C CNN
F 2 "" H 4060 4490 50  0001 C CNN
F 3 "" H 4060 4490 50  0001 C CNN
	1    4060 4490
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR1
U 1 1 59E10209
P 3540 4570
F 0 "#PWR1" H 3540 4320 50  0001 C CNN
F 1 "GND" H 3540 4420 50  0000 C CNN
F 2 "" H 3540 4570 50  0001 C CNN
F 3 "" H 3540 4570 50  0001 C CNN
	1    3540 4570
	1    0    0    -1  
$EndComp
Wire Wire Line
	3540 4570 3540 4530
$Comp
L NAND-Quad U1
U 1 1 59E26706
P 3960 3590
F 0 "U1" H 3960 3890 50  0000 C CNN
F 1 "NAND-Quad" H 3960 3790 50  0000 C CNN
F 2 "footprint" H 3960 2690 50  0001 C CNN
F 3 "datasheet" H 3960 2590 50  0001 C CNN
F 4 "bom" H 3960 2490 50  0001 C CNN "BOM"
	1    3960 3590
	1    0    0    -1  
$EndComp
$Comp
L NAND-Quad U2
U 1 1 59E26C1C
P 7120 3590
F 0 "U2" H 7120 3290 50  0000 C CNN
F 1 "NAND-Quad" H 7120 3380 50  0000 C CNN
F 2 "footprint" H 7120 2690 50  0001 C CNN
F 3 "datasheet" H 7120 2590 50  0001 C CNN
F 4 "bom" H 7120 2490 50  0001 C CNN "BOM"
	1    7120 3590
	-1   0    0    1   
$EndComp
Connection ~ 3540 4530
$Comp
L Motor_Servo M3
U 1 1 59E29295
P 5520 2350
F 0 "M3" H 5490 2120 50  0000 L CNN
F 1 "Motor_Servo" H 5330 2070 50  0000 L TNN
F 2 "" H 5520 2160 50  0001 C CNN
F 3 "" H 5520 2160 50  0001 C CNN
	1    5520 2350
	1    0    0    -1  
$EndComp
Text Label 2900 4650 1    60   ~ 0
GND
Text Label 4770 3490 2    60   ~ 0
GND
Text Label 6480 3490 2    60   ~ 0
GND
Text Label 4490 3690 0    60   ~ 0
U1A_O
Text Label 4270 3630 0    60   ~ 0
2A
Text Label 2790 4710 1    60   ~ 0
M3_3
Text Label 4870 2490 0    60   ~ 0
A1_GND
Text Label 5030 2290 0    60   ~ 0
D10
Text Label 2020 3780 0    60   ~ 0
M3_1
Text Label 1990 3890 0    60   ~ 0
1,2EN
Text Label 4600 3080 0    60   ~ 0
D11
Text Label 3010 2110 3    60   ~ 0
VCC1
Text Label 6300 3080 0    60   ~ 0
A1_+5V
Text Label 4590 3190 0    60   ~ 0
D13
Text Label 1870 4090 0    60   ~ 0
1A,U1A_I
Text Label 3470 3680 0    60   ~ 0
D13
Text Label 3470 3580 0    60   ~ 0
D13
Text Label 5290 4440 0    60   ~ 0
1Y
Text Label 4650 4440 0    60   ~ 0
2Y
Text Label 6140 4450 0    60   ~ 0
3Y
Text Label 5520 4440 0    60   ~ 0
4Y
Text Label 7550 3580 2    60   ~ 0
D7
Text Label 7550 3690 2    60   ~ 0
D7
Text Label 6290 3190 0    60   ~ 0
D7
Text Label 6290 3690 0    60   ~ 0
U2A_O
Text Label 6700 3630 0    60   ~ 0
3A
Text Label 4580 3800 0    60   ~ 0
+9V
Text Label 4550 3290 0    60   ~ 0
M1_1
Text Label 4540 3590 0    60   ~ 0
M1_2
Text Label 6290 3590 0    60   ~ 0
M2_1
Text Label 6290 3290 0    60   ~ 0
M2_2
Text Label 6290 3800 0    60   ~ 0
D5
Text Label 1990 3300 0    60   ~ 0
3,4EN
Text Label 1870 3490 0    60   ~ 0
4A,U2A_I
Text Label 2700 2340 1    60   ~ 0
+9V
Text Notes 8140 7640 0    60   ~ 0
10/14/2017 11:34 PM
$EndSCHEMATC
