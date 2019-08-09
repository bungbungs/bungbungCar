
#include <asf.h>

//! [setup]
//! [buf_length]
#define BUF_LENGTH 20
//! [buf_length]

//! [buffer]
static uint8_t buffer_rx[BUF_LENGTH] = {0x00,};
static uint8_t buffer_expect[BUF_LENGTH] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
		 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13
};
//! [buffer]
//! [var]
volatile bool transfer_complete_spi_slave = false;
//! [var]

//! [dev_inst]
struct spi_module spi_slave_instance;
struct usart_module usart_instance;
//! [dev_inst]
//! [setup]

void configure_spi_slave_callbacks(void);
void configure_spi_slave(void);

//! [callback]
static void spi_slave_callback(struct spi_module *const module)
{
//! [callback_var]
	transfer_complete_spi_slave = true;
//! [callback_var]
}
//! [callback]

//! [conf_callback]
void configure_spi_slave_callbacks(void)
{
//! [reg_callback]
	spi_register_callback(&spi_slave_instance, spi_slave_callback,
			SPI_CALLBACK_BUFFER_RECEIVED);
//! [reg_callback]
//! [en_callback]
	spi_enable_callback(&spi_slave_instance, SPI_CALLBACK_BUFFER_RECEIVED);
//! [en_callback]
}
//! [conf_callback]

//! [configure_spi]
void configure_spi_slave(void)
{
//! [config]
	struct spi_config config_spi_slave;
//! [config]
	/* Configure, initialize and enable SERCOM SPI module */
//! [conf_defaults]
	spi_get_config_defaults(&config_spi_slave);
//! [conf_defaults]
//! [conf_spi_slave_instance]
	config_spi_slave.mode = SPI_MODE_SLAVE;
//! [conf_spi_slave_instance]
//! [conf_preload]
	config_spi_slave.mode_specific.slave.preload_enable = true;
//! [conf_preload]
//! [conf_format]
	config_spi_slave.mode_specific.slave.frame_format = SPI_FRAME_FORMAT_SPI_FRAME;
//! [conf_format]
//! [mux_setting]
	config_spi_slave.mux_setting = CONF_SLAVE_MUX_SETTING;
//! [mux_setting]

	config_spi_slave.pinmux_pad0 = CONF_SLAVE_PINMUX_PAD0;
	config_spi_slave.pinmux_pad1 = CONF_SLAVE_PINMUX_PAD1;
	config_spi_slave.pinmux_pad2 = CONF_SLAVE_PINMUX_PAD2;
	config_spi_slave.pinmux_pad3 = CONF_SLAVE_PINMUX_PAD3;

//! [init]
	spi_init(&spi_slave_instance, CONF_SLAVE_SPI_MODULE, &config_spi_slave);
//! [init]

//! [enable]
	spi_enable(&spi_slave_instance);
//! [enable]

}
//! [configure_spi]

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


int main(void)
{
//! [main_start]
	uint8_t result = 0;
	
	/* Initialize system */
//! [system_init]
	system_init();
	delay_init();
//! [system_init]

//! [run_config]
	configure_spi_slave();
//! [run_config]
//! [run_callback_config]
	configure_spi_slave_callbacks();
//! [run_callback_config]
//! [main_start]
	configuration_usart();

//! [main_use_case]
//! [read]
	spi_read_buffer_job(&spi_slave_instance, buffer_rx, BUF_LENGTH, 0x00);
//! [read]
//! [transf_complete]
	while(!transfer_complete_spi_slave) {
		/* Wait for transfer from master */
	}
//! [transf_complete]

	//printf("%d", buffer_rx[1]);

//! [compare]
	for (uint8_t i = 0; i < BUF_LENGTH; i++) {
		if(buffer_rx[i] != buffer_expect[i]) {
			result++;
		}
	}

	for(uint8_t i = 0; i < 20; i++ )
		printf("%d", buffer_rx[i]);
//! [compare]

//! [inf_loop]
	while (true) {
		/* Infinite loop */
		if (result) {
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			/* Add a short delay to see LED toggle */
			volatile uint32_t delay = 30000;
			while(delay--) {
			}
		} else {
			port_pin_set_output_level(LED_0_PIN, LED_0_ACTIVE);
			delay_ms(100);
			port_pin_set_output_level(LED_0_PIN, !LED_0_ACTIVE);
			//printf("A");
			/* Add a short delay to see LED toggle */
			volatile uint32_t delay = 600000;
			while(delay--) {
			}
		}
	}
//! [inf_loop]
//! [main_use_case]
}
