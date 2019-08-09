
#include <asf.h>
#include "buzzer.h"
#include "color_sensor.h"
#include "infrared_sensor.h"
//#include "spi_master.h"
#include "master_i2c.h"

//usart----------------------------------------------------------------------------------
struct usart_module usart_instance;
void configure_usart(void);



extern bool infrared_buffer[BUF_LENGTH];

//main----------------------------------------------------------------------------------------
int main(void)
{
	
	system_init();
	delay_init();
	
	configure_usart();
	configure_tc();
	
	//configure_spi_master();
	configure_i2c_master();
	
	element r = { 0x00, RED, PRE_NONE };
	element g = { 0x00, GREEN, PRE_NONE };
	element b = { 0x00, BLUE, PRE_NONE };
	
	element biggest = { 0x00, NONE, PRE_NONE };
	element smallest = { 0x00, NONE, PRE_NONE };
	rainbow master_color = 7;
	
	printf("Hi this is MASTER\n");

	basic_pin_setting();
	uint16_t timeout = 0;
	struct i2c_master_packet packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = BUF_LENGTH,
		.data        = infrared_buffer,
		.ten_bit_address = false,
		.high_speed      = false,
		.hs_master_code  = 0x0,
	};
		
	while (true) {
	//	if(!port_pin_get_input_level(BUTTON_0_PIN)) {
	//	spi_select_slave(&spi_master_instance, &slave, true);

		printf("Before write\n");
		
		line_sensor_init();
	
		while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
			
			if (timeout++ == TIMEOUT) {
				break;
			}
		}
		delay_ms(5);
		for(int i = 0; i < BUF_LENGTH; i++)
			printf("%d ", infrared_buffer[i]);
		printf("\n");
			
		//spi_write_buffer_wait(&spi_master_instance, infrared_buffer, BUF_LENGTH);
		//printf("After write\n");
		//spi_select_slave(&spi_master_instance, &slave, false);
		//port_pin_set_output_level(LED_0_PIN, LED0_ACTIVE);
		
		printf("before color\n");	
	//	master_color = result_color();
				//
	//	tcc_reset(&tcc_instance);
	//	configure_tcc(master_color);
				//
//		}
	}
	
}

//usart---------------------------------------------------------------------------
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
	
	while(usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK){}
	usart_enable(&usart_instance);
	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart );
}