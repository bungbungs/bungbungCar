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
struct tc_module tc_instance;
struct tc_config config_tc;
struct usart_module usart_instance;
uint32_t durationA, durationB, durationC;
void configuration_usart(void){
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate=9600;
	config_usart.mux_setting=EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0=EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1=EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2=EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3=EDBG_CDC_SERCOM_PINMUX_PAD3;
	
	while (usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart)!= STATUS_OK);
	usart_enable(&usart_instance);
	stdio_serial_init(&usart_instance,EDBG_CDC_MODULE,&config_usart);
}
void configure_tc(void){
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size=TC_COUNTER_SIZE_16BIT;
	config_tc.clock_source=GCLK_GENERATOR_0;
	config_tc.clock_prescaler=TC_CLOCK_PRESCALER_DIV8;
	config_tc.enable_capture_on_channel[TC_COMPARE_CAPTURE_CHANNEL_0]=true;
	tc_init(&tc_instance, TC3, &config_tc);
	tc_enable(&tc_instance);
}
void HC_SR04_init(void){
	
	struct port_config pin_conf_trig;
	port_get_config_defaults(&pin_conf_trig);
	struct port_config pin_conf_echo;
	port_get_config_defaults(&pin_conf_echo);
	
	pin_conf_trig.direction=PORT_PIN_DIR_OUTPUT;
	pin_conf_trig.input_pull=PORT_PIN_PULL_UP;
	
	pin_conf_echo.direction=PORT_PIN_DIR_INPUT;
	pin_conf_echo.input_pull=PORT_PIN_PULL_UP;
	
	port_pin_set_config(PIN_PA28, &pin_conf_trig);
	port_pin_set_config(PIN_PA13, &pin_conf_echo);
	
	
	port_pin_set_config(PIN_PA23, &pin_conf_echo);
	port_pin_set_config(PIN_PA15, &pin_conf_trig);
	
	port_pin_set_config(PIN_PA14, &pin_conf_echo);
	port_pin_set_config(PIN_PA08, &pin_conf_trig);
	
	port_pin_set_output_level(PIN_PA28, false); //기본 세팅을 false, default값을 모르기 때문에
	delay_ms(50);
	port_pin_set_output_level(PIN_PA28, true);
	delay_ms(10);
	port_pin_set_output_level(PIN_PA28, false);
	uint32_t start, end;
	
	
	tc_start_counter(&tc_instance);
	while(!(port_pin_get_input_level(PIN_PA13))) {}
	start=tc_get_count_value(&tc_instance);
	while(port_pin_get_input_level(PIN_PA13)) {}
	end=tc_get_count_value(&tc_instance);
	durationA=end-start;
	tc_stop_counter(&tc_instance);
	end=0; start=0;
	
	port_pin_set_output_level(PIN_PA15, false);
	delay_ms(50);
	port_pin_set_output_level(PIN_PA15, true);
	delay_ms(10);
	port_pin_set_output_level(PIN_PA15, false);
	
	tc_start_counter(&tc_instance);
	while(!(port_pin_get_input_level(PIN_PA23))){}
	start=tc_get_count_value(&tc_instance);
	while(port_pin_get_input_level(PIN_PA23)) {}
	end=tc_get_count_value(&tc_instance);
	durationB=end-start;
	tc_stop_counter(&tc_instance);
	end=0; start=0;
	
	port_pin_set_output_level(PIN_PA08, false);
	delay_ms(50);
	port_pin_set_output_level(PIN_PA08, true);
	delay_ms(10);
	port_pin_set_output_level(PIN_PA08, false);
	
	tc_start_counter(&tc_instance);
	while(!(port_pin_get_input_level(PIN_PA14))){}
	start=tc_get_count_value(&tc_instance);
	while(port_pin_get_input_level(PIN_PA14)) {}
	end=tc_get_count_value(&tc_instance);
	durationC=end-start;
	tc_stop_counter(&tc_instance);
	end=0; start=0;
}

int main (void)
{
	
	system_init();
	delay_init();
	configure_tc();
	
	configuration_usart();
	
	/* Insert application code here, after the board has been initialized. */

	/* This skeleton code simply sets the LED to the state of the button. */
	while (1) {
		HC_SR04_init();
		delay_ms(500);
		float distanceA=((float)(durationA*0.017));
		printf("A:%f ", distanceA);
		float distanceB=((float)(durationB*0.017));
		printf("B:%f ", distanceB);
		float distanceC=((float)(durationC*0.017));
		printf("C:%f\n", distanceC);
	}
}


