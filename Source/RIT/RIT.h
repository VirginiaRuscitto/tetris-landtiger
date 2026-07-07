#ifndef __RIT_H
#define __RIT_H

#include <stdint.h>

/*lib_RIT.c*/
uint32_t init_RIT(uint32_t RITInterval);
void enable_RIT(void);
void disable_RIT(void);
void reset_RIT(void);
unsigned int get_RIT_value();

/*IRQ_RIT.c*/
void RIT_IRQHandler (void);

#endif