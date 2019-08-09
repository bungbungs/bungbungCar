﻿/*
 * master_i2c.h
 *
 * Created: 2017-04-18 오후 7:49:55
 *  Author: Seunggon
 */ 


#ifndef MASTER_I2C_H_
#define MASTER_I2C_H_

#define SLAVE_ADDRESS 0x12
#define TIMEOUT 1000

#define BUF_LENGTH 3

struct i2c_master_module i2c_master_instance;

void configure_i2c_master(void);

#endif /* MASTER_I2C_H_ */