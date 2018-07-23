# esp32_simple_snmpv2c_agent
IoT: Simple example using SNMPv2c Agent for ESP32 + LwIP 2.0.3 + ESP-IDF 3.0

Project TAGS: SNMP Agent, SNMPv2c, Private MIB, MIB2, MIB-II, ESP32, ESP-IDF, LwIP, Example, Simple.

This may also apply for ESP-8266 library with some lightly changes, however you can't use and replace components.7z but you can
check the files listed in lwip_fixes.md and analise wich ones must be changed in the LwIP component only.


<b>Tested on...</b>

Board: ESP32_Core_board_V2
Version: Framework ESP-IDF 3.0
Version: LWIP 2.0.3

May work on further ESP-IDF versions since the components.7z is the full copy of components in ESP-IDF 3.0 with Lwip component fixed, but you should not update to the new version after merge into framework.

This is an simple example of SNMPv2c Agent w/ private MIB for ESP32.

<b>Author</b>: 

Leandro Silva <@gmail.com>
Student of Computer Engineering from Polytechnic Institute of Leiria - Portugal

My Espressif contact nickname: leandro.adonis86

Please be kind and support my work for further release, any donation is apreciated<br>
at https://www.paypal.me/LeandroAdonis/5 for 5€ or 5$ amount. Any amount you wish,<br>
I will be very gratefull and real happy to see my work worth something to you. Thank you.

<b>Requirements</b>:

Make sure ESP32 is full setup by following the tutorial and running an example for test on link https://dl.espressif.com/doc/esp-idf/latest/get-started/index.html

Note: May need to install adicional drivers in your system on link https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

Updated LwIP component available: http://download.savannah.nongnu.org/releases/lwip/

Related document for LwIP: http://www.nongnu.org/lwip/2_0_x/group__snmp.html

<b>Install</b>:

Uncompress components.7z using 7zip or any other alternative.<br>
Replace all file from genuine framework ESP-IDF 3.0 components path to this components fixed ones.<br>
Copy snmp agent example to your esp examples folder or home directory.<br>
Open terminal, go to snmp agent example path and type command "make menuconfig".<br>
Make sure the LwIP "component config" is exact same as the image below and "Serial flash config" is set to your correct device.<br>
Execute "make flash" to compile into your ESP32.<br>
Use putty to connect to your ESP32 serial.

<b>For server side tools you can use:</b><br>
Nagios, PowerSNMP Free Manager (Windows), SolarWinds SNMPWalk, SNMP MIB Browser (Android).

<b>Make menuconfig options:</b>

$make menuconfig<br>
Component config > LwIP <br>
https://github.com/leandroadonis86/esp32_simple_snmpv2c_agent/blob/master/lwip_component.jpg
<img src="https://raw.githubusercontent.com/leandroadonis86/esp32_simple_snmpv2c_agent/master/lwip_component.jpg">


<b>Fixing alternatives:</b>

Remove "build" folder from compiled project if you apply any changes.<br>
Try "make clean" and then "make flash".<br>
Make sure LwIP options are correct in "make menuconfig".<br>
Use make -d to debug make falty errors.<br>
Make sure lwip component.mk is including snmp library. (is already in components.7z)

<b>File information:</b>

<b>components.7z</b>: Zip File that includes all components from ESP-IDF 3.0 framework with component LwIP fixed files for SNMP.<br>
file scan:
https://www.virustotal.com/#/file/d1d08e381a907ad84ae78d75372f51c6f546f1bc698bc66211c864cbbf898e22/detection



