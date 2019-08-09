/*
 * ultrasonic.c
 *
 * Created: 2017-04-13 오후 5:12:37
 *  Author: Seunggon
 */ 

#include <asf.h>
#include "ultrasonic.h"

void configure_tc(void){
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV8;
	config_tc.enable_capture_on_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = true;
	tc_init(&tc_instance, TC3, &config_tc);
	tc_enable(&tc_instance);
}

void HC_SR04_init(void){
	
	struct port_config pin_conf_trig;
	port_get_config_defaults(&pin_conf_trig);
	
	struct port_config pin_conf_echo;
	port_get_config_defaults(&pin_conf_echo);
	
	pin_conf_trig.direction = PORT_PIN_DIR_OUTPUT;
	pin_conf_trig.input_pull = PORT_PIN_PULL_UP;
	
	pin_conf_echo.direction = PORT_PIN_DIR_INPUT;
	pin_conf_echo.input_pull = PORT_PIN_PULL_UP;
	
	//Head
	port_pin_set_config(GPIO_PIN_ULTRA11, &pin_conf_trig);
	port_pin_set_config(GPIO_PIN_ULTRA12, &pin_conf_echo);
	
	//Left
	port_pin_set_config(GPIO_PIN_ULTRA21, &pin_conf_echo);
	port_pin_set_config(GPIO_PIN_ULTRA22, &pin_conf_trig);
	
	//Right
	port_pin_set_config(GPIO_PIN_ULTRA31, &pin_conf_echo);
	port_pin_set_config(GPIO_PIN_ULTRA32, &pin_conf_trig);
	
}

void ultra(void)
{
	printf("inside ultra\n");

	uint32_t durationR, durationL, durationH;
	uint32_t start, end;
	bool H = false;
	
	//[Head trigger]
	port_pin_set_output_level(GPIO_PIN_ULTRA11, false);
	delay_ms(50);
	port_pin_set_output_level(GPIO_PIN_ULTRA11, true);
	delay_ms(10);
	port_pin_set_output_level(GPIO_PIN_ULTRA11, false);

	//[Head echo]
	tc_start_counter(&tc_instance);
	while(!(port_pin_get_input_level(GPIO_PIN_ULTRA12))) {}
	start = tc_get_count_value(&tc_instance);
	while(port_pin_get_input_level(GPIO_PIN_ULTRA12)) {}
	end = tc_get_count_value(&tc_instance);

	durationH = end - start;
	distanceH = ((float)(durationH * 0.017));
	printf("%f\n", distanceH);
	
	tc_stop_counter(&tc_instance);
	end = 0; start = 0;
	
	//
	// 	//[Left trigger]
	// 	port_pin_set_output_level(GPIO_PIN_ULTRA31, false);
	// 	delay_ms(50);
	// 	port_pin_set_output_level(GPIO_PIN_ULTRA31, true);
	// 	delay_ms(10);
	// 	port_pin_set_output_level(GPIO_PIN_ULTRA31, false);
	//
	// 	//[Left echo]
	// 	tc_start_counter(&tc_instance);
	// 	while(!(port_pin_get_input_level(GPIO_PIN_ULTRA32))){}
	// 	start = tc_get_count_value(&tc_instance);
	// 	while(port_pin_get_input_level(GPIO_PIN_ULTRA32)) {}
	// 	end = tc_get_count_value(&tc_instance);
	//
	// 	durationL = end - start;
	// 	distanceL=((float)(durationL*0.017));
	//
	// 	tc_stop_counter(&tc_instance);
	// 	end=0; start=0;
	//
	//
	// 	//[Right trigger]
	// 	port_pin_set_output_level(GPIO_PIN_ULTRA21, false);
	// 	delay_ms(50);
	// 	port_pin_set_output_level(GPIO_PIN_ULTRA21, true);
	// 	delay_ms(10);
	// 	port_pin_set_output_level(GPIO_PIN_ULTRA21, false);
	//
	// 	//[Right echo]
	// 	tc_start_counter(&tc_instance);
	// 	while(!(port_pin_get_input_level(GPIO_PIN_ULTRA22))){}
	// 	start=tc_get_count_value(&tc_instance);
	// 	while(port_pin_get_input_level(GPIO_PIN_ULTRA22)) {}
	// 	end = tc_get_count_value(&tc_instance);
	//
	// 	durationR = end - start;
	// 	distanceR = ((float)(durationR * 0.017));
	//
	// 	tc_stop_counter(&tc_instance);
	// 	end = 0; start = 0;
	
	//printf("%f, %f, %f", distanceH, distanceL, distanceR);
}
