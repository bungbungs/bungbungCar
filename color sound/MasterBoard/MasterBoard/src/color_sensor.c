/*
 * color_sensor.c
 *
 * Created: 2017-04-05 오후 2:27:43
 *  Author: Seunggon
 */ 

#include <asf.h>
#include "color_sensor.h"

precedence get_label(element i_color, element biggest, element smallest) {
	
	float value = 0.0;
	
	NORM(value, i_color, biggest, smallest);
	
	switch(biggest.color) {
		case RED:
			if(i_color.color == GREEN) {
				if(value > 0.4)
					return MAX;
				else if (value == 0)
					return MIN;
				else
					return MID;
			}
			else {
				if(value > 0.9)
					return MAX;
				else
					return MIN;
			}
		case GREEN:
			if(value > 0.9)
				return MAX;
			else
				return MIN;
		case BLUE:
			if(value > 0.2)
				return MAX;
			else
			return MIN;
		default:
			break;
	}
}

element get_max(element r, element g, element b) {

	element biggest = { 0x00, NONE, PRE_NONE };
	
	biggest.rgb_value = r.rgb_value > g.rgb_value ? r.rgb_value : g.rgb_value;
	
	if(r.rgb_value > g.rgb_value) {
		biggest.rgb_value = r.rgb_value;
		biggest.color = RED;
		biggest.order = MAX;
	}
	else {
		biggest.rgb_value = g.rgb_value;
		biggest.color = GREEN;
		biggest.order = MAX;
	}
	
	if(b.rgb_value > biggest.rgb_value) {
		biggest.rgb_value = b.rgb_value;
		biggest.color = BLUE;
		biggest.order = MAX;
	}
	
	return biggest;
}

element get_min(element r, element g, element b) {

	element smallest = { 0x00, NONE, PRE_NONE };
	
	smallest.rgb_value = r.rgb_value < g.rgb_value ? r.rgb_value : g.rgb_value;
	
	if(r.rgb_value < g.rgb_value) {
		smallest.rgb_value = r.rgb_value;
		smallest.color = RED;
		smallest.order = MIN;
	}
	else {
		smallest.rgb_value = g.rgb_value;
		smallest.color = GREEN;
		smallest.order = MIN;
	}
	
	if(b.rgb_value < smallest.rgb_value) {
		smallest.rgb_value = b.rgb_value;
		smallest.color = BLUE;
		smallest.order = MIN;
	}
	
	return smallest;
}

rainbow transferhz(element r, element g, element b, element biggest)
{
	rainbow note=none;

	switch(biggest.color) {
		case RED:
		if(g.order == MIN && b.order == MIN)
		note = 0;
		else if(g.order == MID && b.order == MIN)
		note = 1;
		else if(g.order == MAX && b.order == MIN)
		note = 2;
		else
		note = none;
		break;
		case GREEN:
		if(r.order == MIN && b.order == MIN)
		note = 3;
		else if(r.order == MIN && b.order == MAX)
		note = 5;
		else
		note = none;
		break;
		case BLUE:
		if(r.order == MIN && g.order == MIN)
		note = 4;
		else if (r.order == MAX && g.order == MIN)
		note = 6;
		else if (r.order == MIN && g.order == MAX)
		note = 5;
		else
		note = 7;
	}
	return note;
}

char* get_color_string(rainbow n) {
	
	switch(n) {
		case red: return "red";
		case orange: return "orange";
		case yellow: return "yellow";
		case green: return "green";
		case blue: return "blue";
		case sky: return "sky";
		case purple: return "purple";
		default: return "NONE";
	}
}

uint8_t get_color(COLOR color, OUT_NUM out) {

	uint8_t result = 0x00;
	uint8_t	start  = 0x00;
	uint8_t	end	   = 0x00;

	switch(color) {
		case RED:
		// get Red element
		rgb_pin_setting(RED);
		break;
		case GREEN:;
		// get Green element
		rgb_pin_setting(GREEN);
		break;
		case BLUE:
		// get Blue element
		rgb_pin_setting(BLUE);
		break;
		default:
		// error
		break;
	}
	
	
	tc_start_counter(&tc_instance);
	if(out == OUT_NUM_1) {	// Color 1
		while(port_pin_get_input_level(TERM_OUT_1));		// wait until input level is LOW
		while(!(port_pin_get_input_level(TERM_OUT_1)));		// if the signal's level is HIGH, start timer
		start  = tc_get_count_value(&tc_instance);
		
		while(port_pin_get_input_level(TERM_OUT_1));		// if the signal's level is LOW, stop timer
		end    = tc_get_count_value(&tc_instance);
	}
	else if(out == OUT_NUM_2)
	{	// Color 2
		while(port_pin_get_input_level(TERM_OUT_2));		// wait until input level is LOW
		while(!(port_pin_get_input_level(TERM_OUT_2)));		// if the signal's level is HIGH, start timer
		start  = tc_get_count_value(&tc_instance);
		
		while(port_pin_get_input_level(TERM_OUT_2));		// if the signal's level is LOW, stop timer
		end    = tc_get_count_value(&tc_instance);
	}
	result = end - start;
	

	
	tc_stop_counter(&tc_instance);
	start = 0;
	end = 0;

	return result;
}

rainbow result_color (void)
{
	// color 1
	element r1 = { 0x00, RED, PRE_NONE };
	element g1 = { 0x00, GREEN, PRE_NONE };
	element b1 = { 0x00, BLUE, PRE_NONE };

	// color 2
	element r2 = { 0x00, RED, PRE_NONE };
	element g2 = { 0x00, GREEN, PRE_NONE };
	element b2 = { 0x00, BLUE, PRE_NONE };
	
	// color
	element r = { 0x00, RED, PRE_NONE };
	element g = { 0x00, GREEN, PRE_NONE };
	element b = { 0x00, BLUE, PRE_NONE };

	element biggest = { 0x00, NONE, PRE_NONE };
	element smallest = { 0x00, NONE, PRE_NONE };
	rainbow n = none;
	
		// get the RGB value from color 1
	r1.rgb_value = DURATION / get_color(RED, OUT_NUM_1);
	delay_ms(100);
	g1.rgb_value = DURATION / get_color(GREEN, OUT_NUM_1);
	delay_ms(100);
	b1.rgb_value = DURATION / get_color(BLUE, OUT_NUM_1);
	delay_ms(100);
		
		
	// get the RGB value from color 2
	r2.rgb_value = DURATION / get_color(RED, OUT_NUM_2);
	delay_ms(100);
	g2.rgb_value = DURATION / get_color(GREEN, OUT_NUM_2);
	delay_ms(100);
	b2.rgb_value = DURATION / get_color(BLUE, OUT_NUM_2);
	delay_ms(100);

	r.rgb_value = (r1.rgb_value+r2.rgb_value)/2;
	g.rgb_value = (g1.rgb_value+g2.rgb_value)/2;
	b.rgb_value = (b1.rgb_value+b2.rgb_value)/2;

	// get the Largest value of RGB
/*		if(r1.rgb_value>r2.rgb_value){
		r.rgb_value = r1.rgb_value;
	}
	else
		r.rgb_value = r2.rgb_value;
			
	if(g1.rgb_value>g2.rgb_value){
		g.rgb_value = g1.rgb_value;
	}
	else
		g.rgb_value = g2.rgb_value;
		
	if(b1.rgb_value>b2.rgb_value){
		b.rgb_value = b1.rgb_value;
	}
	else
		b.rgb_value = b2.rgb_value;   */
		
	// get the level of each color
	biggest  = get_max(r, g, b);
	smallest = get_min(r, g, b);
		
	r.order = get_label(r, biggest, smallest);
	g.order = get_label(g, biggest, smallest);
	b.order = get_label(b, biggest, smallest);
		
	n = transferhz(r, g, b, biggest);	
		
	printf("%3d, %3d, %3d | %3d, %3d, %3d | %3d, %3d, %3d | %d | %s\n", r1.rgb_value, g1.rgb_value, b1.rgb_value, r2.rgb_value, g2.rgb_value, b2.rgb_value, r.rgb_value, g.rgb_value, b.rgb_value,n, get_color_string(n));
		
	return n;
}

void basic_pin_setting() {
	
	struct port_config pin_conf_in;
	struct port_config pin_conf_out;
	
	port_get_config_defaults(&pin_conf_in);
	port_get_config_defaults(&pin_conf_out);
	
	pin_conf_in.direction	= PORT_PIN_DIR_INPUT;
	pin_conf_in.input_pull	= PORT_PIN_PULL_UP;
	
	pin_conf_out.direction	= PORT_PIN_DIR_OUTPUT;
	pin_conf_out.input_pull	= PORT_PIN_PULL_UP;
	
	port_pin_set_config(TERM_S0, &pin_conf_out);
	port_pin_set_config(TERM_S1, &pin_conf_out);
	port_pin_set_config(TERM_S2, &pin_conf_out);
	port_pin_set_config(TERM_S3, &pin_conf_out);
	port_pin_set_config(TERM_OUT_1, &pin_conf_in);
	port_pin_set_config(TERM_OUT_2, &pin_conf_in);
	
	port_pin_set_output_level(TERM_S0, HIGH);
	port_pin_set_output_level(TERM_S1, HIGH);
	port_pin_set_output_level(TERM_S2, LOW);
	port_pin_set_output_level(TERM_S3, LOW);

}

void rgb_pin_setting(COLOR color) {

	switch(color) {
		case RED:	// s2: L, s3: L
		port_pin_set_output_level(TERM_S2, LOW);
		port_pin_set_output_level(TERM_S3, LOW);
		break;
		case GREEN:	// s2: H, s3: H
		port_pin_set_output_level(TERM_S2, HIGH);
		port_pin_set_output_level(TERM_S3, HIGH);
		break;
		case BLUE:	// s2: L, s3: H
		port_pin_set_output_level(TERM_S2, LOW);
		port_pin_set_output_level(TERM_S3, HIGH);
		break;
		case CLEAR:	// s2: H, s3: L  (Clear)
		port_pin_set_output_level(TERM_S2, HIGH);
		port_pin_set_output_level(TERM_S3, LOW);
	}
}

void configure_tc(void) {
	
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size		= TC_COUNTER_SIZE_16BIT;
	config_tc.clock_source		= GCLK_GENERATOR_3;
	config_tc.clock_prescaler	= TC_CLOCK_PRESCALER_DIV8;
	config_tc.enable_capture_on_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = true;
	
	tc_init(&tc_instance, TC3, &config_tc);
	tc_enable(&tc_instance);

}