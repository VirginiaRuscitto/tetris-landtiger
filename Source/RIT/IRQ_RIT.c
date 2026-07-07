#include "LPC17xx.h"
#include "RIT.h"
#include "../timer/timer.h"
#include "../led/led.h"
#include "../joystick/joystick.h"
#include "tetris.h"

volatile int down_0=0;
volatile int down_1=0;
volatile int down_2=0;
volatile int release_0=0;
volatile int release_1=0;
volatile int release_2=0;

volatile int up=0;
volatile int down=0;
volatile int right=0;
volatile int left=0;

#define SHIFT_DELAY 3

void RIT_IRQHandler(void) 
{			
	if(down_1 !=0) {	/* KEY1 */
		down_1++;
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ /* button pressed */
			switch(down_1){
				case 2:
					if (game.state != PLAY) {
						disable_RIT();
						start_game();
						enable_RIT();
          } else if (game.state == PLAY) {
						disable_RIT();
						pause_game();
						enable_RIT();
					}
					release_1=1;
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			if(release_1){
				release_1=0;
			}			
			down_1=0;	
			NVIC_EnableIRQ(EINT1_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 22); 
		}
	}

	if(down_2 !=0) {			/* KEY2 */
		down_2++;
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ /* button pressed */
			switch(down_2){
				case 2:	
					disable_RIT();
					hard_drop();
					enable_RIT();
					release_2=1;
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			if(release_2){
				release_2=0;
			}	
			down_2=0;		
			NVIC_EnableIRQ(EINT2_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 24);     			 /* External interrupt 0 pin selection  */
		}
	}

	if(joystick_dir(JOYSTICK_UP)){
		up++;
		switch(up){
			case 1:	
				disable_RIT();				
				rotate();	
				enable_RIT();
				break;
			default:
				if(up % SHIFT_DELAY == 0) {
					disable_RIT();
					rotate();	
					enable_RIT();
        }
		}
	}
	else {
		up=0;
	}
	
	if(joystick_dir(JOYSTICK_DOWN)){
		down++;
		switch(down){
			case 1:			
				disable_RIT();				
				soft_drop_on();
				enable_RIT();
				break;
	  	default:
				break;
		}
	}
	else{
		if(down > 0) {
			soft_drop_off();
    }
		down=0;
	}

	if(joystick_dir(JOYSTICK_RIGHT)){
		right++;
		switch(right){
			case 1:		
				disable_RIT();				
				move_right();	
				enable_RIT();
				break;
			default:
				if(right % SHIFT_DELAY == 0) { //spostamento di 1 blocco a destra ogni 150ms che il joystick è verso destra (dopo il primo spostamento verso dx che avviene dopo soli 50 ms)
					disable_RIT();
					move_right();
					enable_RIT();
        }
				break;
		}
	}
	else {
		right=0;
	}

	if(joystick_dir(JOYSTICK_LEFT)){
		left++;
		switch(left){
			case 1:		
				disable_RIT();				
				move_left();
				enable_RIT();
				break;
			default:
				if(left % SHIFT_DELAY == 0) {
					disable_RIT();
					move_left();
					enable_RIT();
        }
				break;
		}
	}
	else {
		left=0;
	}
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
	return;
}