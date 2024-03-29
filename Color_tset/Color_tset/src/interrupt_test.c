﻿/*
 * interrupt_test.c
 *
 * Created: 2017-03-22 오후 4:46:14
 *  Author: hoyong
 */ 
#include <asf.h>
#include "conf_qs_events_interrupt_hook.h"

	//! [setup]

static volatile uint32_t event_count = 0;

void event_counter(struct events_resource *resource);

void configure_event_channel(struct events_resource *resource)
{
	//! [setup_1]
	struct events_config config;
	//! [setup_1]

	//! [setup_2]
	events_get_config_defaults(&config);
	//! [setup_2]

	//! [setup_3]
	config.generator      = CONF_EVENT_GENERATOR;
	config.edge_detect    = EVENTS_EDGE_DETECT_RISING;
	config.path           = EVENTS_PATH_SYNCHRONOUS;
	config.clock_source   = GCLK_GENERATOR_0;
	//! [setup_3]

	//! [setup_4]
	events_allocate(resource, &config);
	//! [setup_4]
}

void configure_event_user(struct events_resource *resource)
{
	//! [setup_5]
	events_attach_user(resource, CONF_EVENT_USER);
	//! [setup_5]
}

void configure_tc(struct tc_module *tc_instance)
{
	//! [setup_6]
	struct tc_config config_tc;
	struct tc_events config_events;
	//! [setup_6]

	//! [setup_7]
	tc_get_config_defaults(&config_tc);
	//! [setup_7]

	//! [setup_8]
	config_tc.counter_size    = TC_COUNTER_SIZE_8BIT;
	config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ;
	config_tc.clock_source    = GCLK_GENERATOR_1;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV64;
	//! [setup_8]

	//! [setup_9]
	tc_init(tc_instance, CONF_TC_MODULE, &config_tc);
	//! [setup_9]

	//! [setup_10]
	config_events.generate_event_on_overflow = true;
	tc_enable_events(tc_instance, &config_events);
	//! [setup_10]

	//! [setup_11]
	tc_enable(tc_instance);
	//! [setup_11]

}

void configure_event_interrupt(struct events_resource *resource,
		struct events_hook *hook)
{
	//! [setup_12]
	events_create_hook(hook, event_counter);
	//! [setup_12]

	//! [setup_13]
	events_add_hook(resource, hook);
	events_enable_interrupt_source(resource, EVENTS_INTERRUPT_DETECT);
	//! [setup_13]
}


	//! [setup_14]
void event_counter(struct events_resource *resource)
{
	if(events_is_interrupt_set(resource, EVENTS_INTERRUPT_DETECT)) {
		port_pin_toggle_output_level(LED_0_PIN);

		event_count++;
		events_ack_interrupt(resource, EVENTS_INTERRUPT_DETECT);

	}
}
	//! [setup_14]

	//! [setup]


void attach_interrupt(void)
{

	//! [setup_init]
	struct tc_module       tc_instance;
	struct events_resource example_event;
	struct events_hook     hook;

	system_interrupt_enable_global();

	configure_event_channel(&example_event);
	configure_event_user(&example_event);
	configure_event_interrupt(&example_event, &hook);
	configure_tc(&tc_instance);
	//! [setup_init]

	//! [main]
	delay_ms(100);
	port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
	//! [main_1]
	while (events_is_busy(&example_event)) {
		port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		/* Wait for channel */
	};
	//! [main_1]

	//! [main_2]
	tc_start_counter(&tc_instance);
	//! [main_2]

	while (true) {
		/* Nothing to do */
		port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
		delay_ms(100);
		port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
		delay_ms(100);
		if(event_count == 10) break;
	}

	//! [main]
}
