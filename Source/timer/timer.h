#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

/*init_timer.c*/
uint32_t init_timer(uint8_t timer_num, uint32_t Prescaler, uint8_t MatchReg, uint8_t SRImatchReg, uint32_t TimerInterval);
void enable_timer(uint8_t timer_num);
void disable_timer(uint8_t timer_num);
void reset_timer(uint8_t timer_num);
unsigned int get_timer_value(uint8_t timer_num);
float get_timer_value_sec(uint8_t timer_num, uint32_t timer_freq);
uint32_t is_timer_enabled (uint8_t timer_num);
void power_on_timer2();
void power_on_timer3();

/*IRQ_timer.c*/
void TIMER0_IRQHandler (void);
void TIMER1_IRQHandler (void);
void TIMER2_IRQHandler (void);
void TIMER3_IRQHandler (void);

#endif