
#include <asf.h>
#include "infrared_sensor.h"

bool infrared_buffer[BUF_LENGTH] = {0};

bool get_infrared_signal(const uint8_t pin_num)
{
	return port_pin_get_input_level(pin_num) == HIGH;
}

void line_sensor_init(void)
{
	struct port_config signal_pin1;
	port_get_config_defaults(&signal_pin1);
	
	signal_pin1.direction = PORT_PIN_DIR_INPUT;
	signal_pin1.input_pull = PORT_PIN_PULL_DOWN;
	
	port_pin_set_config(GPIO_PIN_INFRARED1, &signal_pin1);
	
	struct port_config signal_pin2;
	port_get_config_defaults(&signal_pin2);
	
	signal_pin2.direction = PORT_PIN_DIR_INPUT;
	signal_pin2.input_pull = PORT_PIN_PULL_DOWN;
	
	port_pin_set_config(GPIO_PIN_INFRARED2, &signal_pin2);
	
	struct port_config signal_pin3;
	port_get_config_defaults(&signal_pin3);
	
	signal_pin3.direction = PORT_PIN_DIR_INPUT;
	signal_pin3.input_pull = PORT_PIN_PULL_DOWN;
	
	port_pin_set_config(GPIO_PIN_INFRARED3, &signal_pin3);
	
// 	struct port_config signal_pin4;
// 	port_get_config_defaults(&signal_pin4);
// 	
// 	signal_pin4.direction = PORT_PIN_DIR_INPUT;
// 	signal_pin4.input_pull = PORT_PIN_PULL_DOWN;
// 	
// 	port_pin_set_config(PIN_PA23, &signal_pin4);
// 	
// 	struct port_config signal_pin5;
// 	port_get_config_defaults(&signal_pin5);
// 	
// 	signal_pin5.direction = PORT_PIN_DIR_INPUT;
// 	signal_pin5.input_pull = PORT_PIN_PULL_DOWN;
// 	
// 	port_pin_set_config(PIN_PA28, &signal_pin5);
	
	infrared_buffer[0] = port_pin_get_input_level(GPIO_PIN_INFRARED1);
	infrared_buffer[1] = port_pin_get_input_level(GPIO_PIN_INFRARED2);
	infrared_buffer[2] = port_pin_get_input_level(GPIO_PIN_INFRARED3);
	
// 	infrared_buffer[3] = port_pin_get_input_level(PIN_PA23);
// 	infrared_buffer[4] = port_pin_get_input_level(PIN_PA28);
	
	//infrared_buffer[0] = 0;
	//infrared_buffer[1] = 1;
	//infrared_buffer[2] = 0;
	//infrared_buffer[3] = 1;
	//infrared_buffer[4] = 1;
	//
	//printf("%d, %d, %d, %d, %d\n",
	//port_pin_get_input_level(PIN_PA13),
	//port_pin_get_input_level(PIN_PA14),
	//port_pin_get_input_level(PIN_PA15),
	//port_pin_get_input_level(PIN_PA23),
	//port_pin_get_input_level(PIN_PA28));
}