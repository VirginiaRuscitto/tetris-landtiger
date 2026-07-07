#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdio.h>

/*lib_button*/
void BUTTON_init(void);

/*IRQ_button*/
void EINT0_IRQHandler(void);
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);

#endif