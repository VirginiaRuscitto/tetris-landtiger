#include "button.h"
#include "LPC17xx.h"

/*----------------------------------------------------------------------------
  Function that initializes button GPIO pins and external interrupts
 *----------------------------------------------------------------------------*/
void BUTTON_init(void) {

	LPC_PINCON->PINSEL4    |= (1 << 20);	  	/* External interrupt 0 pin selection */
	LPC_GPIO2->FIODIR      &= ~(1 << 10);    	/* PORT2.10 defined as input          */

	LPC_PINCON->PINSEL4    |= (1 << 22);     	/* External interrupt 0 pin selection */
	LPC_GPIO2->FIODIR      &= ~(1 << 11);    	/* PORT2.11 defined as input          */
  
	LPC_PINCON->PINSEL4    |= (1 << 24);     	/* External interrupt 0 pin selection */
	LPC_GPIO2->FIODIR      &= ~(1 << 12);    	/* PORT2.12 defined as input          */

	LPC_SC->EXTMODE = 0x7;
	
	NVIC_EnableIRQ(EINT1_IRQn);            
	NVIC_SetPriority(EINT2_IRQn, 1);
	
	NVIC_EnableIRQ(EINT2_IRQn);
	NVIC_SetPriority(EINT1_IRQn, 1);		 
}
