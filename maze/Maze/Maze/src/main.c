
#include <asf.h>

#include "motor.h"
#include "ultrasonic.h"

struct usart_module usart_instance;

void configure_usart(void);

//main--------------------------------------------------------------------
int main(void)
{
	uint32_t a=0, b=0;
	
	system_init();
	delay_init();
	configure_usart();

	configure_tc();
	configure_tcc();

	//configure_i2c_slave();
	//configure_spi_slave();

	enum i2c_slave_direction dir;
	struct i2c_slave_packet packet = {
		.data_length = DATA_LENGTH,
		.data        = read_buffer,
	};

	printf("Hi this is slave\n");
	
	HC_SR04_init();

	while (true) {

		ultra();
		
		//while(spi_read_buffer_wait(&spi_slave_instance, buffer_rx, BUF_LENGTH, 0x00) != STATUS_OK) {
		///* Wait for transfer from the master */
		//}
		
		while(distanceH < DISTANCE_MAX) {
			//printf("in  while: %6.2f, %6.2f, %6.2f\n", distanceH, distanceL, distanceR);
			if(distanceH == V_ERROR || distanceL == V_ERROR || distanceR == V_ERROR) {
				servo(LEFTBACK, RIGHTBACK);
				delay_us(1500);
				servo(LEFTGO, RIGHTBACK);
				delay_us(1000);
			}
			else if(distanceL - distanceR > 0) {// turn left
				servo(LEFTBACK, RIGHTGO);
				delay_us(500);
			}
			else if (distanceL - distanceR < 0) { // turn right
				servo(LEFTGO, RIGHTBACK);
				delay_us(500);
			}
			else { // go back
				servo(LEFTBACK, RIGHTBACK);
				delay_us(1000);
			}
			ultra();
		}
		
		if(distanceR - distanceL > 0) {
			servo(LEFTGO, RIGHTBACK);
			delay_us(700);
			servo(LEFTGO, RIGHTGO);
			delay_us(700);
			servo(LEFTBACK, RIGHTGO);
			delay_us(700);
		}
		else if (distanceR - distanceL < 0) {
			servo(LEFTBACK, RIGHTGO);
			delay_us(700);
			servo(LEFTGO, RIGHTGO);
			delay_us(700);
			servo(LEFTGO, RIGHTBACK);
			delay_us(700);
		}
		servo(LEFTGO, RIGHTGO);
		delay_us(1000);

}


void configure_usart(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate = 9600; //14400,19200, 115200
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	
	while(usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK)
	{}
	usart_enable(&usart_instance);
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart );
}