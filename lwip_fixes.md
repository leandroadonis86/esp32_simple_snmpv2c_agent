<b>LwIP 2.0.3 component briefing changes for ESP32 ESP-IDF 3.0</b>:

<b>lwipopts.h</b>: added #define LWIP_SNMP 1

<b>stat.h</b>: "struct stats_mib2 mib2;" and "#if MIB2_STATS" must exist.

<b>opt.h</b>: "#define MIB2_STATS 1" must set to 1. LWIP_IPV4 verify if it's active. For
	IPv6 not included in example use var LWIP_IPV6 and 6LOWPAN use LWIP_6LOWPAN
	with value 1.

<b>ip_addr.h</b>: add "IP4_ADDR_ANY4" following example in same file "IP4_ADDR_ANY" 
	like this "#define IP4_ADDR_ANY4 (ip_2_ip4(&ip_addr_any))". Other 
	possible add vars "IP_IS_V4_VAL" and "IP_IS_V4" for IPv4.

<b>arch.h</b>: Compare and Add speciefied code present in arch.h file from the 
	component present in  http://download.savannah.nongnu.org/releases/lwip/

<b>ethernetif.c</b>: in function "NETIF_INIT_SNMP()" you see var 
	LINK_SPEED_OF_YOUR_NETIF_IN_BPS change for desire interface speed 100 
	as an example.

<b>snmp_opts.h</b>: configure "SNMP_COMMUNITY", "SNMP_COMMUNITY_WRITE", "SNMP_COMMUNITY_TRAP"
	"SNMP_TRAP_DESTINATIONS" with value 1 to be able SNMPwalk. Optional 
	configure "SNMP_LWIP_MIB2_SYSDESC", "SNMP_LWIP_MIB2_SYSNAME", 
	"SNMP_LWIP_MIB2_SYSCONTACT" those are settable functions executed on 
	simple_snmp_agent.c example. Switch SNMPv1 to SNMPv2c change var
	"#define SNMP_COMMUNITY" “public" to “community”.

<b>lwip component.mk</b>: verify section "COMPONENT_ADD_INCLUDEDIRS" include “apps/snmp” and
	for source verify section "COMPONENT_SRCDIRS" add also “apps/snmp”.
