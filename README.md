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

Please be kind and support my work for further release, any donation is apreciated
at https://www.paypal.me/LeandroAdonis/5 for 5€ or 5$ amount. Any amount you wish,
I will be very gratefull and real happy to see my work worth something to you. Thank you.

<b>Requirements</b>:

Make sure you ESP32 is full setup by following the tutorial and running an example for test on link https://dl.espressif.com/doc/esp-idf/latest/get-started/index.html

Note: May need to install adicional drivers in your system on link https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

<b>Install</b>:

Uncompress components.7z using 7zip or anyother alternative.
Replace all file from genuine framework ESP-IDF 3.0 components path to this components fixed ones.
Copy snmp agent example to your esp examples folder or home directory.
Open terminal, go to example path and type command "make menuconfig".
Make sure the LwIP "component config" is exact same as the image below and "Serial flash config" is set to your correct device.
Execute "make flash" to compile into your ESP32.
Use putty to connect to your ESP32 serial.

For server side tools you can use:
Nagios, PowerSNMP Free Manager (Windows), SolarWinds SNMPWalk, SNMP MIB Browser (Android).

Make menuconfig options:



<b>Fixing alternatives</b>:

Remove "build" folder from compiled projet if you apply any changes.
Try "make clean" and then "make flash".
Make sure LwIP options are correct in "make menuconfig".
Use make -d to debug make falty errors.
Make sure lwip component.mk is including snmp library. (is already in components.7z)






