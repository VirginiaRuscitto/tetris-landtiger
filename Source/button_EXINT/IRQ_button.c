#include "button.h"
#include "LPC17xx.h"
#include "../led/led.h"
#include "../timer/timer.h"

extern int down_0;
extern int down_1;
extern int down_2;

/*----------------------------------------------------------------------------
  External interrupt 1 (EINT1) handler
 *----------------------------------------------------------------------------*/
void EINT1_IRQHandler (void)
{
	down_1 = 1;
	NVIC_DisableIRQ(EINT1_IRQn);	
	LPC_PINCON->PINSEL4 &= ~(1 << 22); /* GPIO pin selection*/
	LPC_SC->EXTINT &= (1 << 1); /* clear pending interrupt*/
}

/*----------------------------------------------------------------------------
  External interrupt 2 (EINT2) handler
 *----------------------------------------------------------------------------*/
void EINT2_IRQHandler (void)	
{
	down_2 = 1;
	NVIC_DisableIRQ(EINT2_IRQn);
	LPC_PINCON->PINSEL4 &= ~(1 << 24); /* GPIO pin selection*/
	LPC_SC->EXTINT &= (1 << 2); /* clear pending interrupt*/    
}
