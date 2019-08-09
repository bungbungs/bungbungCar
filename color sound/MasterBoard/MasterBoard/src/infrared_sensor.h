/*
 * infrared_sensor.h
 *
 * Created: 2017-04-05 오후 2:17:17
 *  Author: Seunggon
 */ 

#ifndef INFRARED_SENSOR_H_
#define INFRARED_SENSOR_H_

#define GPIO_PIN_INFRARED1 PIN_PA08
#define GPIO_PIN_INFRARED2 PIN_PA14
#define GPIO_PIN_INFRARED3 PIN_PA22

#define BUF_LENGTH 3

void line_sensor_init(void);
bool get_infrared_signal(const uint8_t);

#endif /* INFRARED_SENSOR_H_ */