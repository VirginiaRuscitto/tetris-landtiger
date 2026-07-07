#include <stdio.h>
#include "LPC17xx.h"                  
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "GLCD/GLCD.h"
#include "tetris.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main (void) {
  
	//Init
	SystemInit();
  BUTTON_init();
	joystick_init();
	LCD_Initialization();
	init_RIT(0x004C4B40); //50 msec=0x004C4B40 1/4=0x0007A120
	enable_RIT();
	init_timer(0, 0, 0, 3, 0x047878C0); //in order to have a numerical value to set srand()
	enable_timer(0);
	init_game();

	LPC_SC->PCON |= 0x1;		/* power-down	mode */
	LPC_SC->PCON &= 0xFFFFFFFD;						
		
  while (1) { 		/* Loop forever */	
		__ASM("wfi");
  }

}
