/*
 * color_sensor.h
 *
 * Created: 2017-04-05 오후 2:26:11
 *  Author: Seunggon
 */ 


#ifndef COLOR_SENSOR_H_
#define COLOR_SENSOR_H_

#define DURATION	15000
#define TERM_S0		PIN_PA14
#define TERM_S1		PIN_PA08
#define TERM_S2		PIN_PA13
#define TERM_S3		PIN_PA28
#define TERM_OUT_1	PIN_PA15
#define TERM_OUT_2	PIN_PA23

#define NORM(value, i_color, biggest, smallest) { value = (float) (i_color.rgb_value - smallest.rgb_value) / (biggest.rgb_value - smallest.rgb_value); }


typedef enum {
	OUT_NUM_1,		// TERM_OUT_1
	OUT_NUM_2		// TERM_OUT_2
} OUT_NUM;

typedef enum { NONE, RED, GREEN, BLUE, CLEAR } COLOR;
typedef enum { PRE_NONE, MIN, MID, MAX } precedence;
typedef enum { red, orange,	yellow,	green, blue, sky, purple, none } rainbow;

typedef struct {
	uint8_t		rgb_value;
	COLOR		color;
	precedence	order;
} element;

struct tc_module	tc_instance;
struct tc_config	config_tc;

precedence get_label(element i_color, element biggest, element smallest);
element get_max(element r, element g, element b);
element get_min(element r, element g, element b);
rainbow transferhz(element r, element g, element b, element biggest);
char* get_color_string(rainbow n);
uint8_t get_color(COLOR color, OUT_NUM out);
rainbow result_color(void);
void basic_pin_setting(void);
void rgb_pin_setting(COLOR color);

void configure_tc(void);

#endif /* COLOR_SENSOR_H_ */