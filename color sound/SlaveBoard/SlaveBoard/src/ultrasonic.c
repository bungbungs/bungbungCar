/*
 * ultrasonic.c
 *
 * Created: 2017-04-13 오후 5:12:37
 *  Author: Seunggon
 */ 

#include <asf.h>
#include "ultrasonic.h"
#include "motor.h"

void configure_tc(void){
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV8;
	config_tc.enable_capture_on_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = true;
	tc_init(&tc_instance, TC3, &config_tc);
	tc_enable(&tc_instance);
}

void configure_ultrasonic_pin(void){
	
	struct port_config pin_conf_trig;
	port_get_config_defaults(&pin_conf_trig);
	
	struct port_config pin_conf_echo;
	port_get_config_defaults(&pin_conf_echo);
	
	pin_conf_trig.direction = PORT_PIN_DIR_OUTPUT;
	pin_conf_trig.input_pull = PORT_PIN_PULL_UP;
	
	pin_conf_echo.direction = PORT_PIN_DIR_INPUT;
	pin_conf_echo.input_pull = PORT_PIN_PULL_UP;
	
	//Trig
	port_pin_set_config(GPIO_PIN_ULTRA_TRIG, &pin_conf_trig);
	
	//Head
	port_pin_set_config(GPIO_PIN_ULTRA_ECHO1, &pin_conf_echo);
	
	//Left
	//port_pin_set_config(GPIO_PIN_ULTRA11, &pin_conf_trig);
	port_pin_set_config(GPIO_PIN_ULTRA_ECHO2, &pin_conf_echo);
	
	//Right
	//port_pin_set_config(GPIO_PIN_ULTRA11, &pin_conf_trig);
	port_pin_set_config(GPIO_PIN_ULTRA_ECHO3, &pin_conf_echo);
	
}

void get_distances(void)
{
	printf("inside ultra\n");

	uint32_t durationH, durationL, durationR;
	uint32_t start, end;
	bool H = false;
	
	//[Head trigger]
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, false);
	delay_ms(50);
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, true);
	delay_ms(10);
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, false);

	//[Head echo]
	tc_start_counter(&tc_instance);
	while(!(port_pin_get_input_level(GPIO_PIN_ULTRA_ECHO1))) {}
	start = tc_get_count_value(&tc_instance);
	while(port_pin_get_input_level(GPIO_PIN_ULTRA_ECHO1)) {}
	end = tc_get_count_value(&tc_instance);

	durationH = end - start;
	distanceH = ((float)(durationH * 0.017));
	if(distanceH > DISTANCE_MAX + 50 || distanceH < 0.2)
		distanceH = V_ERROR;
	printf("%f\n", distanceH);
	
	tc_stop_counter(&tc_instance);
	end = 0; start = 0;
	
	
	//[Left trigger]
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, false);
	delay_ms(50);
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, true);
	delay_ms(10);
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, false);
	
	//[Left echo]
	tc_start_counter(&tc_instance);
	while(!(port_pin_get_input_level(GPIO_PIN_ULTRA_ECHO2))){}
	start = tc_get_count_value(&tc_instance);
	while(port_pin_get_input_level(GPIO_PIN_ULTRA_ECHO2)) {}
	end = tc_get_count_value(&tc_instance);
	
	durationL = end - start;
	distanceL = ((float)(durationL * 0.017));
	if(distanceL > DISTANCE_MAX + 50 || distanceL < 0.2)
		distanceL = V_ERROR;
	tc_stop_counter(&tc_instance);
	end=0; start=0;
	//printf(" L(%6.2f)\n", distanceL);
	
	//[Right trigger]
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, false);
	delay_ms(50);
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, true);
	delay_ms(10);
	port_pin_set_output_level(GPIO_PIN_ULTRA_TRIG, false);
	
	//[Right echo]
	tc_start_counter(&tc_instance);
	while(!(port_pin_get_input_level(GPIO_PIN_ULTRA_ECHO3))){}
	start = tc_get_count_value(&tc_instance);
	while(port_pin_get_input_level(GPIO_PIN_ULTRA_ECHO3)) {}
	end = tc_get_count_value(&tc_instance);
	
	durationR = end - start;
	distanceR = ((float)(durationR * 0.017));
	if(distanceR > DISTANCE_MAX + 50 || distanceR < 0.2)
		distanceR = V_ERROR;

	tc_stop_counter(&tc_instance);
	end = 0; start = 0;
	//printf(" R(%6.2f), ", distanceR);
	
	printf("%f, %f, %f", distanceH, distanceL, distanceR);
}
