/*
 * motor.h
 *
 * Created: 2017-04-13 오후 5:09:24
 *  Author: Seunggon
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#define CONF_PWM_CHANNEL 2
#define CONF_PWM_OUTPUT 2

#define CONF_PWM_CHANNEL1 3
#define CONF_PWM_OUTPUT1 3

#define DISTANCE_MAX 25

#define RIGHT false
#define LEFT true

#define RIGHTGO 0x1F00
#define RIGHTBACK 0x1000
#define LEFTGO 0x1000
#define LEFTBACK 0x2000
#define STOP 0x0000

#define TURN_TIME	2000
#define DETAIL_TIME 30
#define V_ERROR		999

struct tcc_config config_tcc;
struct tcc_module tcc_instance;

void configure_tcc(void);
void servo(uint32_t left, uint32_t right);
void Not_find(void);
void Rfind(void);



#endif /* MOTOR_H_ */