/*
 * master_i2c.c
 *
 * Created: 2017-04-18 오후 7:49:29
 *  Author: Seunggon
 */ 

#include <asf.h>
#include "master_i2c.h"

void configure_i2c_master(void)
{
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);

	config_i2c_master.pinmux_pad0    = EDBG_I2C_SERCOM_PINMUX_PAD0 ;
	config_i2c_master.pinmux_pad1    = EDBG_I2C_SERCOM_PINMUX_PAD1;

	config_i2c_master.buffer_timeout = 10000;

	
	i2c_master_init(&i2c_master_instance, CONF_I2C_MASTER_MODULE, &config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
}