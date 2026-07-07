#include "LPC17xx.h"
#include "timer.h"
#include "tetris.h"
#include "music.h"

void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1){
		move_down();  
		LPC_TIM0->IR = 1;  
	}  
  return;
}

void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1){
		music_timer2();
		LPC_TIM2->IR = 1;  
	}  
  return;
}

void TIMER3_IRQHandler (void)
{
	if(LPC_TIM3->IR & 1){
		music_timer3();
		LPC_TIM3->IR = 1;  
	}  
  return;
}