#include "my_mib.h"

/*
 * SNMP general function prototype
 * Add here any function to be usable in specific private MIB node.
 */
static s16_t get_leds_value(struct snmp_node_instance* instance, void* value);
static s16_t get_switches_value(struct snmp_node_instance* instance, void* value);
static s16_t get_single_value(struct snmp_node_instance* instance, void* value);
static snmp_err_t set_leds_value(struct snmp_node_instance* instance, u16_t len, void *value);


/* led .1.3.6.1.4.1.26381.1.1 */
//static const struct snmp_scalar_node leds_node = SNMP_SCALAR_CREATE_NODE_READONLY(1, SNMP_ASN1_TYPE_INTEGER, get_leds_value); // ready-only example
static const struct snmp_scalar_node leds_node = SNMP_SCALAR_CREATE_NODE(1, SNMP_NODE_INSTANCE_READ_WRITE, SNMP_ASN1_TYPE_INTEGER, get_leds_value, NULL, set_leds_value); //read-and-write example
/* switch .1.3.6.1.4.1.26381.1.2 */
static const struct snmp_scalar_node switches_node = SNMP_SCALAR_CREATE_NODE_READONLY(2, SNMP_ASN1_TYPE_INTEGER, get_switches_value);
/* xgpio .1.3.6.1.4.1.26381.1.3 */
static const struct snmp_scalar_node single_node = SNMP_SCALAR_CREATE_NODE_READONLY(3, SNMP_ASN1_TYPE_INTEGER, get_single_value);

/* all private nodes  .1.3.6.1.4.1.26381.1 .. (.1,.2,.3) .0 */
static const struct snmp_node* const gpio_nodes[] = { &switches_node.node.node, &leds_node.node.node, &single_node.node.node };
static const struct snmp_tree_node gpio_tree_node = SNMP_CREATE_TREE_NODE(1, gpio_nodes);

/* define private mib */
static const u32_t my_base_oid[] = { 1, 3, 6, 1, 4, 1, 26381, 1 };
const struct snmp_mib gpio_mib = SNMP_MIB_CREATE(my_base_oid, &gpio_tree_node.node);


/* leds value .1.3.6.1.4.1.26381.1.1.0 */
static s16_t get_leds_value(struct snmp_node_instance* instance, void* value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = (u32_t) gpio_get_level((gpio_num_t) *(&leds)); /* get structure values */
	return sizeof(*uint_ptr);
}

/* switches value .1.3.6.1.4.1.26381.1.2.0 */
static s16_t get_switches_value(struct snmp_node_instance* instance, void* value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = (u32_t) gpio_get_level((gpio_num_t) *(&switches)); /* get structure values */
	return sizeof(*uint_ptr);
}

/* xgpio value .1.3.6.1.4.1.26381.1.3.0 */
static s16_t get_single_value(struct snmp_node_instance* instance, void* value) {
	u32_t *uint_ptr = (u32_t*) value;
	*uint_ptr = *xgpio;
	return sizeof(*uint_ptr);
}

static snmp_err_t set_leds_value(struct snmp_node_instance* instance, u16_t len, void *value) {
	u32_t val = *((u32_t*)value);
	gpio_set_level((gpio_num_t) *(&leds), (u32_t) val); /* write a value in struture */
	return SNMP_ERR_NOERROR;
}
