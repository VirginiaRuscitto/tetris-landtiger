#ifndef __LED_H
#define __LED_H

#include <stdio.h>

/*lib_led*/
void LED_init(void);
void LED_deinit(void);

/*funct_led*/
void LED_On (unsigned int num);
void LED_Off (unsigned int num);
void LED_Out(unsigned int value);
void LED_AllOn(void);
void LED_AllOff(void);
void LED_Out_Range(unsigned int value, unsigned char from_led_num, unsigned char to_led_num);

#define LED4 7
#define LED5 6
#define LED6 5
#define LED7 4
#define LED8 3
#define LED9 2
#define LED10 1
#define LED11 0

#endif

