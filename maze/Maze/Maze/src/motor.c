/*
 * motor.c
 *
 * Created: 2017-04-13 오후 5:09:04
 *  Author: Seunggon
 */ 

#include <asf.h>
#include "ultrasonic.h"
#include "motor.h"

bool direction=false, find=false;

void configure_tcc(void)
{
	tcc_reset(&tcc_instance);
	tcc_get_config_defaults(&config_tcc,TCC0);
	
	config_tcc.counter.clock_source = GCLK_GENERATOR_0;
	config_tcc.counter.period = 0xFFFF;
	config_tcc.counter.clock_prescaler = 1;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	
	//config_tcc.compare.match[CONF_PWM_CHANNEL] = left;
	//config_tcc.compare.match[CONF_PWM_CHANNEL1] =right;


	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT] = PIN_PA18F_TCC0_WO2;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT] = MUX_PA18F_TCC0_WO2;

	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT1] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT1] = PIN_PA19F_TCC0_WO3;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT1] = MUX_PA19F_TCC0_WO3;

	
	tcc_init(&tcc_instance, TCC0, &config_tcc);
	tcc_enable(&tcc_instance);
	
}

void servo(uint32_t left, uint32_t right)
{
		tcc_reset(&tcc_instance);
		config_tcc.compare.match[CONF_PWM_CHANNEL] = left;
		config_tcc.compare.match[CONF_PWM_CHANNEL1] =right;
		tcc_init(&tcc_instance, TCC0,&config_tcc);
		tcc_enable(&tcc_instance);

}
void Not_find(void)
{
	ultra();
	//if(listenSPI())
	//	return;
	if(distanceL <= distanceR) direction = RIGHT;
	else direction = LEFT;
	while(1){
			ultra();
			/*
			if(listenSPI())
				return;
				*/
			if(distanceH<DISTANCE_MAX)
			{
				while(distanceL<DISTANCE_MAX&&distanceR<DISTANCE_MAX)
				{
					servo(LEFTBACK, RIGHTBACK);
					ultra();
					/*
					if(listenSPI())
						return;
						*/
				}
				if(distanceL<DISTANCE_MAX) direction=RIGHT;
				else if(distanceR<DISTANCE_MAX) direction=LEFT;
				
				if(direction==RIGHT){
					servo(LEFTGO, RIGHTBACK);
					direction=LEFT;
				}
				else if(direction==LEFT){
					servo(LEFTBACK, RIGHTGO);
					direction=RIGHT; 
				}
				/*
				if(listenSPI())
					return;
					*/
				delay_ms(2000);		
			}
			else servo(LEFTGO, RIGHTGO);
	}
}
// void Rfind(void)
// {
// 	while(1)
// 	{
// 		listenSPI();
// 		printf("%d %d %d\n", buffer_rx[2], buffer_rx[1], buffer_rx[0]);
// 		if(buffer_rx[1]==1)
// 		{
// 			servo(LEFTGO, RIGHTGO);
// 			printf("go\n");
// 		}
// 		else if(buffer_rx[2]==1)
// 		{
// 			servo(STOP, RIGHTGO);
// 			printf("left\n");
// 		}                              
// 		else if(buffer_rx[0]==1) 
// 		{
// 			servo(LEFTGO, STOP);
// 			printf("right\n");
// 		}
// 		else
// 		{
// 			servo(STOP, STOP);
// 			printf("stop\n");
// 		}
// 		delay_ms(200);
// 		//servo(STOP, STOP);
// 		delay_ms(200);
// 	}
// }
