#include "RIT.h"
#include "LPC17xx.h"

/*----------------------------------------------------------------------------
  Function that enables the RIT
 *----------------------------------------------------------------------------*/
void enable_RIT( void )
{
	LPC_RIT->RICTRL |= (1<<3);	
	return;
}

/*----------------------------------------------------------------------------
  Function that disables the RIT
 *----------------------------------------------------------------------------*/
void disable_RIT( void )
{
	LPC_RIT->RICTRL &= ~(1<<3);	
	return;
}

/*----------------------------------------------------------------------------
  Function that resets the RIT counter
 *----------------------------------------------------------------------------*/
void reset_RIT( void )
{
	LPC_RIT->RICOUNTER = 0;          			// Set count value to 0
	return;
}

/*----------------------------------------------------------------------------
  Function that initializes the RIT
 *----------------------------------------------------------------------------*/
uint32_t init_RIT ( uint32_t RITInterval )
{
	LPC_SC->PCLKSEL1  &= ~(3<<26);
	LPC_SC->PCLKSEL1  |=  (1<<26);   			// RIT Clock = CCLK
	LPC_SC->PCONP     |=  (1<<16);   			// Enable power for RIT
	
	LPC_RIT->RICOMPVAL = RITInterval;     // Set match value		
	LPC_RIT->RICTRL    = (1<<1) |    			// Enable clear on match	
											 (1<<2) ;		 			// Enable timer for debug	
	LPC_RIT->RICOUNTER = 0;          			// Set count value to 0
	
	NVIC_EnableIRQ(RIT_IRQn);
	NVIC_SetPriority(RIT_IRQn,2);
	return (0);
}

/*----------------------------------------------------------------------------
  Function that returns the current RIT counter value
 *----------------------------------------------------------------------------*/
unsigned int get_RIT_value() {
	return LPC_RIT->RICOUNTER;
}

