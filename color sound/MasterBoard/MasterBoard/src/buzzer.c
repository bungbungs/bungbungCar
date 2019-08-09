/*
 * buzzer.c
 *
 * Created: 2017-04-05 오후 2:41:17
 *  Author: Seunggon
 */ 

#include <asf.h>
#include "buzzer.h"

uint16_t sound[7]= {doe, rae, mi, fa, sol, la, ti};

void configure_tcc(int i)
{
	tcc_get_config_defaults(&config_tcc, TCC0);
	
	config_tcc.counter.clock_source = GCLK_GENERATOR_0;
	config_tcc.counter.period= sound[i];
	//0x0670
	config_tcc.counter.clock_prescaler = 1;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	
	config_tcc.compare.match[CONF_PWM_CHANNEL] = config_tcc.counter.period*3/4;

	
	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT] = PIN_PA18F_TCC0_WO2;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT] = MUX_PA18F_TCC0_WO2;

	
	tcc_init(&tcc_instance, TCC0, &config_tcc);
	tcc_enable(&tcc_instance);
	
}