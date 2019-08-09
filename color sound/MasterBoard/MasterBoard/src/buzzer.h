/*
 * buzzer.h
 *
 * Created: 2017-04-05 오후 2:36:19
 *  Author: Seunggon
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#define CONF_PWM_CHANNEL 2
#define CONF_PWM_OUTPUT 2

#define doe 0x3BDF //261
#define rae 0x34DF //294
#define mi 0x2F7F //330
#define fa 0x2C7F //349
#define sol 0x277F //392
#define la 0x237F //440
#define ti 0x1F7F //494

struct tcc_module tcc_instance;
struct tcc_config config_tcc;

void configure_tcc(int i);


#endif /* BUZZER_H_ */