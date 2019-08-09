
#include <asf.h>

#include "motor.h"
#include "i2c_slave.h"
//#include "spi_slave.h"
#include "ultrasonic.h"

//extern bool buffer_rx[BUF_LENGTH] = {0x00};
extern uint8_t read_buffer[DATA_LENGTH];

struct usart_module usart_instance;

void configure_usart(void);

//main--------------------------------------------------------------------
int main(void)
{
	system_init();
	delay_init();	
	configure_usart();

	configure_tc();
	configure_tcc();

	configure_i2c_slave();

	printf("Hi this is slave\n");
	
	configure_ultrasonic_pin();

	//finding black line------------------------
	//black line founded------------------------
	while (true) {
		i2c_listen();
		Rfind();
	}	

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