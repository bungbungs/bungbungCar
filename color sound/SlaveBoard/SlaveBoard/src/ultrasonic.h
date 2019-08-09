/*
 * ultrasonic.h
 *
 * Created: 2017-04-13 오후 5:12:09
 *  Author: Seunggon
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define  GPIO_PIN_ULTRA_TRIG PIN_PA28
#define  GPIO_PIN_ULTRA_ECHO1 PIN_PA13

//#define  GPIO_PIN_ULTRA21 PIN_PA08
#define  GPIO_PIN_ULTRA_ECHO2 PIN_PA14

//#define  GPIO_PIN_ULTRA31 PIN_PA15
#define  GPIO_PIN_ULTRA_ECHO3 PIN_PA23

float distanceH, distanceL, distanceR, distanceH2;

struct tc_module tc_instance;
struct tc_config config_tc;

void configure_tc(void);
void configure_ultrasonic_pin(void);
void get_distances(void);

#endif /* ULTRASONIC_H_ */