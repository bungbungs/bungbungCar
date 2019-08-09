/*
 * slave_i2c.c
 *
 * Created: 2017-04-18 오후 8:05:13
 *  Author: jhikm_000
 */ 
#include <asf.h>
#include "slave_i2c.h"

uint8_t read_buffer[DATA_LENGTH];

void configure_i2c_slave(void)
{
	struct i2c_slave_config config_i2c_slave;
	i2c_slave_get_config_defaults(&config_i2c_slave);

	config_i2c_slave.address        = SLAVE_ADDRESS;
	config_i2c_slave.address_mode   = I2C_SLAVE_ADDRESS_MODE_MASK;

	config_i2c_slave.pinmux_pad0    = EDBG_I2C_SERCOM_PINMUX_PAD0 ;
	config_i2c_slave.pinmux_pad1    = EDBG_I2C_SERCOM_PINMUX_PAD1;

	config_i2c_slave.buffer_timeout = 1000;

	i2c_slave_init(&i2c_slave_instance, CONF_I2C_SLAVE_MODULE, &config_i2c_slave);
	i2c_slave_enable(&i2c_slave_instance);
}

void i2c_listen(void)
{
	printf("Before read\n");
	while(i2c_slave_read_packet_wait(&i2c_slave_instance, &packet)!= STATUS_OK)
	{
		for(int i = 0; i < DATA_LENGTH; i++)
		{
			printf("%d", packet.data[i]);
		}
		printf("\n");
	}
	printf("After read\n");
}