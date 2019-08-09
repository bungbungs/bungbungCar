/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

// DEFINE ---------------------------------------------------------------------
#define DURATION	15000
#define TERM_S0		PIN_PA14
#define TERM_S1		PIN_PA08
#define TERM_S2		PIN_PA13
#define TERM_S3		PIN_PA28
#define TERM_OUT	PIN_PA15
#define TERM_LED	PIN_PA23

#define NORM(value, i_color, biggest, smallest) { value = (float) (i_color.rgb_value - smallest.rgb_value) / (biggest.rgb_value - smallest.rgb_value); }

// ENUM, STRUCTURE ------------------------------------------------------------
typedef enum { NONE, RED, GREEN, BLUE, CLEAR } COLOR;
typedef enum { PRE_NONE, MIN, MID, MAX } precedence;
typedef enum { red, orange,	yellow,	green, blue, sky, purple, none } rainbow;

typedef struct {
	uint8_t		rgb_value;
	COLOR		color;
	precedence	order;
} element;

// DECLARATION ----------------------------------------------------------------
uint8_t get_color(COLOR color);
void basic_pin_setting();
void rgb_pin_setting(COLOR color);
void configure_usart(void);
void configure_tc(void);
precedence get_label(element i_color, element biggest, element smallest);
element get_max(element r, element g, element b);
element get_min(element r, element g, element b);
rainbow transferhz(element r, element g, element b, element biggest);
char* get_color_string(rainbow n);

// GLOBAL VARIABLE ------------------------------------------------------------
struct usart_module usart_instance;
struct tc_module	tc_instance;
struct tc_config	config_tc;

int main (void)
{
	/* color sensor */ 
	system_init();
	delay_init();
	configure_tc();	
	configure_usart();
	
	element r = { 0x00, RED, PRE_NONE };
	element g = { 0x00, GREEN, PRE_NONE };
	element b = { 0x00, BLUE, PRE_NONE };
	
	element biggest = { 0x00, NONE, PRE_NONE };
	element smallest = { 0x00, NONE, PRE_NONE };
	rainbow n = none;

	basic_pin_setting();	
	
	while(1) {
	
		r.rgb_value = DURATION / get_color(RED);
		delay_ms(100);
		g.rgb_value = DURATION / get_color(GREEN);
		delay_ms(100);
		b.rgb_value = DURATION / get_color(BLUE);
		delay_ms(100);

		biggest  = get_max(r, g, b);
		smallest = get_min(r, g, b);

	/*
		if(biggest.color == RED) {
			r.order = MAX;
			if(g.rgb_value > b.rgb_value) {
				g.order = MID;
				b.order = MIN;
			}
			else {
				g.order = get_label(g, biggest, smallest);
				b.order = get_label(b, biggest, smallest);
			}
		}
		else { */
			r.order = get_label(r, biggest, smallest);
			g.order = get_label(g, biggest, smallest);
			b.order = get_label(b, biggest, smallest);
	//	}
		
		n = transferhz(r, g, b, biggest);
		
		printf("%3d, %3d, %3d | %3d, %3d, %3d | %d | %s\n", r.rgb_value, g.rgb_value, b.rgb_value, r.order, g.order, b.order, n, get_color_string(n));
	
	}
}

// color -----------------------------------------------------------------
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
				note = red;
			else if(g.order == MID && b.order == MIN)
				note = orange;
			else if(g.order == MAX && b.order == MIN)
				note = yellow;
			else
				note = none;
			break;
		case GREEN:
			if(r.order == MIN && b.order == MIN)
				note = green;
			else if(r.order == MIN && b.order == MAX)
				note = sky;
			else
				note = none;
			break;
		case BLUE:
			if(r.order == MIN && g.order == MIN)
				note = blue;
			else if (r.order == MAX && g.order == MIN)
				note = purple;
			else if (r.order == MIN && g.order == MAX)
				note = sky;
			else
				note = none;
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

uint8_t get_color(COLOR color) {

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
	
	while(port_pin_get_input_level(TERM_OUT));
	while(!(port_pin_get_input_level(TERM_OUT)));
	start  = tc_get_count_value(&tc_instance);
	
	while(port_pin_get_input_level(TERM_OUT));
	end    = tc_get_count_value(&tc_instance);
	
	result = end - start;
	
	tc_stop_counter(&tc_instance);
	start = 0;
	end = 0;
	
	return result;
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
	port_pin_set_config(TERM_OUT, &pin_conf_in);
	port_pin_set_config(TERM_LED, &pin_conf_out);
	
	port_pin_set_output_level(TERM_S0, HIGH);
	port_pin_set_output_level(TERM_S1, HIGH);
	port_pin_set_output_level(TERM_S2, LOW);
	port_pin_set_output_level(TERM_S3, LOW);
	port_pin_set_output_level(TERM_LED, LOW);
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

void configure_usart(void) {
	
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate = 115200; //14400,19200, 115200
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	
	while(usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK);
	
	usart_enable(&usart_instance);
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart );
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
// -----------------------------------------------------------------------

