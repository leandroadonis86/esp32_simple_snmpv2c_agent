/**
 * @file
 * Simple SNMPv2c Agent Example for ESP32 by Leandro Silva
 * Tested on... 
 * Board: ESP32_Core_board_V2
 * Version: Framework ESP-IDF 3.0
 * Version: LWIP 2.0.3
 *
 */
 
/*
 * Copyright (c) July 2018 Leandro Silva <@gmail.com>
 * Student of Computer Engineering from Polytechnic Institute of Leiria - Portugal
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This is a simple example of SNMPv2c Agent w/ private MIB for ESP32.
 * 
 * Author: Leandro Silva <@gmail.com>
 * Student of Computer Engineering from Polytechnic Institute of Leiria - Portugal
 *
 * Please be kind and support my work for further release, any donation is apreciated
 * at https://www.paypal.me/LeandroAdonis/5 for 5€ or 5$ amount. Any amount you wish, 
 * I will be very gratefull and real happy to see my work worth something to you. Thank you.
 *
 */

/* library for the app */
#include "simple_snmp_agent.h"
/* library for static IP */
#include "lwip/sockets.h"

/*
 * TODO: Debug gpio mib status
 * Gives information of atual gpio status in terminal. 1 or true to on, 0 or false to off.
*/
#define DEBUG_GPIO_STATUS 1

/* 
 * TODO: Setup WiFi 
 * Define DEFAULT_SSID your SSID Network and DEFAULT_PWD the password,
 * open WiFi in DEFAULT_PWD use "" or set your password for WPA secure.
*/
#define DEFAULT_SSID "PiAccessPoint"
#define DEFAULT_PWD ""

/* 
 * TODO: Setup your Network (if needed static IP)
 * Only necessary if you need Static IP in your ESP or if DHCP in 
 * router is not activated. DEVICE_USE_STATIC_IP 1\true use static 0\false dynamic,
 * DEVICE_IP define you ESP32 IP, DEVICE_GW the gateway or router IP, 
 * DEVICE_NETMASK your network mask /24 default.
*/
#define DEVICE_USE_STATIC_IP	false
#define DEVICE_IP          "172.16.0.210"
#define DEVICE_GW          "172.16.0.254"
#define DEVICE_NETMASK     "255.255.255.0"

/*
 * TODO: Setup SNMP server
 * Define a specific address to send SNMP broadcast package.
*/
#define SNMP_SERVER_IP "172.16.0.250"

static const char *TAG = "simple_snmp_agent.c";

//function prototypes in this file
static void initialize_gpio(void *pvParameter);
static void initialize_snmp(void);
static void initialize_wifi(void);
static esp_err_t event_handler(void *ctx, system_event_t *event);


/* 
 * ----- TODO: Global variables for SNMP Trap vvv
 * Define your own vars SNMP_SYSDESCR for System Description, SNMP_SYSCONTACT 
 * for your contact mail, SNMP_SYSNAME for your system name, SNMP_SYSLOCATION
 * for your location. Also consider the size of each string in _LEN functions.
*/
static const struct snmp_mib *my_snmp_mibs[] = { &mib2, &gpio_mib };
//1.3.6.1.2.1.1.1.0
const u8_t * SNMP_SYSDESCR = (u8_t*) "simple_snmp_agent";		
const u16_t SNMP_SYSDESCR_LEN = sizeof("simple_snmp_agent");
//1.3.6.1.2.1.1.4.0
u8_t * SNMP_SYSCONTACT = (u8_t*) "yourmail@contact.com";		
u16_t SNMP_SYSCONTACT_LEN = sizeof("yourmail@contact.com");
//1.3.6.1.2.1.1.5.0
u8_t * SNMP_SYSNAME = (u8_t*) "ESP32_Core_board_V2";							
u16_t SNMP_SYSNAME_LEN = sizeof("ESP32_Core_board_V2");
//1.3.6.1.2.1.1.6.0
u8_t * SNMP_SYSLOCATION = (u8_t*) "Your Institute or Company"; 		
u16_t SNMP_SYSLOCATION_LEN = sizeof("Your Institute or Company");
/* 
 * ----- TODO: Global variables for SNMP Trap ^^^
*/

/* buffer for snmp service */
u16_t snmp_buffer = 64;

/* relation between mib node and gpio, exist in my_gpio.h */
u32_t *leds, *switches, *xgpio;

/**
 * This is the main fuction that includes an error check for the system boot,
 * one task creation for the static function initialize_gpio() in this source.
 * It calls the function initialize_snmp() to do the actual setup of the SNMP Agent and
 * initialize_wifi() to connect local network. 
 */
void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
	xTaskCreate(&initialize_gpio, "initialize_gpio", 2048, NULL, 5, NULL);
    initialize_snmp();
	initialize_wifi();
	
	ESP_LOGI(TAG, "app_main() reloaded.");
}

/**
 * This function is optional and only for support GPIO to be settable throw private MIB.
 *
 * This function should be passed as a parameter to xTaskCreate since it's
 * a static function and the operative system needs to operate asynchronously.
 *
 * @param pvParameter any struture may needed in this thread for specific purpose.
 *
 * @note Steps to use GPIO in private MIB should be:
 *       1. Initialize gpio by gpio_pad_select_gpio() and set info direction
 *		 	gpio_set_direction() as GPIO_MODE_OUTPUT or GPIO_MODE_INPUT;
 *		 2. In while(1) cycle use gpio_set_level() to change the gpio state,
 *			use a pointer to change the value to same as in gpio_level to make
 *			possible updating mib;
 *		 3. Specify your updating interval by using vTaskDelay() and notify console
 *			using ESP_LOGI().
 */
static void initialize_gpio(void *pvParameter)
{	
	// TODO: define a pointer to update your mib...
	u32_t gpiovalue = 0;
	xgpio = &gpiovalue;
	
	// TODO: define your gpio's in struture as your needs...
	leds = (u32_t*) GPIO_NUM_13;
	switches = (u32_t*) GPIO_NUM_14; //as example
	
	// TODO: initialize GPIO. test it using: one led and connect in GPIO 13 and GND.
	gpio_pad_select_gpio(GPIO_NUM_13);
	// TODO: define direction GPIO_MODE_OUTPUT or GPIO_MODE_INPUT
	gpio_set_direction(GPIO_NUM_13, GPIO_MODE_OUTPUT);
	
	ESP_LOGI(TAG, "initialize_gpio() finished.");
	while(1) {
		/* change gpio state to ON */
		gpio_set_level(GPIO_NUM_13, 1);	
			gpiovalue = 1;
			
#if DEBUG_GPIO_STATUS
			ESP_LOGI(TAG, "Gpio: %d, mib_value: %d", (int) *(&leds), (int) *xgpio);
#endif
			/* set delay interval to 5 seconds */
			vTaskDelay(5000 / portTICK_RATE_MS);

		/* change gpio state to OFF */
		gpio_set_level(GPIO_NUM_13, 0);
			gpiovalue = 0;
			
#if DEBUG_GPIO_STATUS
			ESP_LOGI(TAG, "Gpio: %d, mib_value: %d", (int) *(&leds), (int) *xgpio);
#endif
			/* set delay interval to 5 seconds */
			vTaskDelay(5000 / portTICK_RATE_MS);
	}
}

/**
 * Should be called at the beginning of the program to set up the snmp agent.
 *
 * @note You can look updated instructions in the link below
 * 		 http://www.nongnu.org/lwip/2_0_x/group__snmp.html
 */
static void initialize_snmp(void)
{
	
	snmp_mib2_set_syscontact(SNMP_SYSCONTACT, &SNMP_SYSCONTACT_LEN, snmp_buffer);
	snmp_mib2_set_syslocation(SNMP_SYSLOCATION, &SNMP_SYSLOCATION_LEN, snmp_buffer);
	snmp_set_auth_traps_enabled(ENABLE);
	snmp_mib2_set_sysdescr(SNMP_SYSDESCR, &SNMP_SYSDESCR_LEN);
	snmp_mib2_set_sysname(SNMP_SYSNAME, &SNMP_SYSNAME_LEN, snmp_buffer);
	
	ip_addr_t gw = { 0 };
    ipaddr_aton(SNMP_SERVER_IP,&gw);
	
	snmp_trap_dst_ip_set(TRAP_DESTINATION_INDEX, &gw);
	snmp_trap_dst_enable(TRAP_DESTINATION_INDEX, ENABLE);
	snmp_set_mibs(my_snmp_mibs, LWIP_ARRAYSIZE(my_snmp_mibs));
	
    snmp_init();
	ESP_LOGI(TAG, "initialize_snmp() finished.");
	
}

/**
 * Should be the last function called at app_main(). It makes your ESP 
 * establish connection to your WiFi local network with static or dynamic IP.
 * Define your varibles in "TODO: Setup WiFi" and "TODO: Setup your Network" 
 * section on top of this file.
 *
 */
static void initialize_wifi(void) 
{
    tcpip_adapter_init();
	
#if DEVICE_USE_STATIC_IP
	tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA); // Don't run a DHCP client
	tcpip_adapter_ip_info_t ipInfo;

	inet_pton(AF_INET, DEVICE_IP, &ipInfo.ip);
	inet_pton(AF_INET, DEVICE_GW, &ipInfo.gw);
	inet_pton(AF_INET, DEVICE_NETMASK, &ipInfo.netmask);
	tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
#endif
	
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = DEFAULT_SSID,
            .password = DEFAULT_PWD
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
	ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "initialize_wifi() finished.");
}

static esp_err_t event_handler(void *ctx, system_event_t *event) 
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
            ESP_LOGI(TAG, "Got IP: %s\n",
                     ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            break;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    default:
        break;
    }
    return ESP_OK;
}
