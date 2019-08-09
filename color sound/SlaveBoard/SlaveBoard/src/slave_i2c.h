/*
 * slave_i2c.h
 *
 * Created: 2017-04-19 오전 9:52:50
 *  Author: Seunggon
 */ 


#ifndef SLAVE_I2C_H_
#define SLAVE_I2C_H_

#define DATA_LENGTH 3
#define BUF_LENGTH 3
#define SLAVE_ADDRESS 0x12

struct i2c_slave_module i2c_slave_instance;
struct i2c_slave_packet packet = {
	.data_length = DATA_LENGTH,
	.data        = read_buffer,
};

void configure_i2c_slave(void);
void i2c_listen(void);


#endif /* SLAVE_I2C_H_ */