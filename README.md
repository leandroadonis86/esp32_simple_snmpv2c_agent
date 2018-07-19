# esp32_simple_snmpv2c_agent
IoT: Simple example using SNMPv2c Agent for ESP32 + LwIP 2.0.3 + ESP-IDF 3.0

<b>Tested on...</b>

Board: ESP32_Core_board_V2
Version: Framework ESP-IDF 3.0
Version: LWIP 2.0.3

May work on further ESP-IDF versions since the components are full copy of from ESP-IDF 3.0, you should not update to the new version after merge it to your framework.

This is an simple example of SNMPv2c Agent w/ private MIB for ESP32.

<b>Author</b>: 
Leandro Silva <@gmail.com>
Student of Computer Engineering from Polytechnic Institute of Leiria - Portugal

Please be kind and support my work for further release, any donation is apreciated<br>
at https://www.paypal.me/LeandroAdonis/5 for 5€ or 5$ amount. Any amount you wish,<br>
I will be very gratefull and real happy to see my work worth something to you. Thank you.

<b>Requirements</b>:

Make sure you ESP32 is full setup by following the tutorial and running an example for test on link https://dl.espressif.com/doc/esp-idf/latest/get-started/index.html

Note: May need to install adicional drivers in your system on link https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

<b>Install</b>:

Uncompress components.7z using 7zip or anyother alternative.<br>
Replace all file from genuine framework ESP-IDF 3.0 components path to this components fixed ones.<br>
Copy snmp agent example to your esp examples folder or home directory.<br>
Open terminal, go to example path and type command "make menuconfig".<br>
Make sure the LwIP "component config" is exact same as the image below and "Serial flash config" is set to your correct device.<br>
Execute "make flash" to compile into your ESP32.<br>
Use putty to connect to your ESP32 serial.

<b>For server side tools you can use:</b><br>
Nagios, PowerSNMP Free Manager (Windows), SolarWinds SNMPWalk, SNMP MIB Browser (Android).

<b>Make menuconfig options:</b>
$make menuconfig<br>
Component congig > LwIP<br>
<img src="
https://github.com/leandroadonis86/esp32_simple_snmpv2c_agent/blob/master/lwip_component.jpg" />


<b>Fixing alternatives</b>:

Remove "build" folder from compiled projet if you apply any changes.<br>
Try "make clean" and then "make flash".<br>
Make sure LwIP options are correct in "make menuconfig".<br>
Use make -d to debug make falty errors.<br>
Make sure lwip component.mk is including snmp library. (is already in components.7z)






