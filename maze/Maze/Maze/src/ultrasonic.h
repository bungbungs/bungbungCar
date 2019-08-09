/*
 * ultrasonic.h
 *
 * Created: 2017-04-13 오후 5:12:09
 *  Author: Seunggon
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define  GPIO_PIN_ULTRA11 PIN_PA28
#define  GPIO_PIN_ULTRA12 PIN_PA13

#define  GPIO_PIN_ULTRA21 PIN_PA08
#define  GPIO_PIN_ULTRA22 PIN_PA14

#define  GPIO_PIN_ULTRA31 PIN_PA15
#define  GPIO_PIN_ULTRA32 PIN_PA23

float distanceH, distanceR, distanceL, distanceH2;

struct tc_module tc_instance;
struct tc_config config_tc;

void configure_tc(void);
void HC_SR04_init(void);
void ultra(void);

#endif /* ULTRASONIC_H_ */