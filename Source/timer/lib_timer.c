#include "LPC17xx.h"
#include "timer.h"

/*----------------------------------------------------------------------------
  Function that enables the specified timer
 *----------------------------------------------------------------------------*/
void enable_timer(uint8_t timer_num)
{
  if (timer_num == 0){
		LPC_TIM0->TCR = 1;
  } else if (timer_num == 1){
		LPC_TIM1->TCR = 1;
 	} else if (timer_num == 2){
		LPC_TIM2->TCR = 1;
	}else if (timer_num == 3) {
		LPC_TIM3->TCR = 1;
	}	
  	return;
}

/*----------------------------------------------------------------------------
  Function that disables the specified timer
 *----------------------------------------------------------------------------*/
void disable_timer(uint8_t timer_num)
{
  if (timer_num == 0){
		LPC_TIM0->TCR = 0;
  } else if (timer_num == 1){
		LPC_TIM1->TCR = 0;
  } else if (timer_num == 2){
		LPC_TIM2->TCR = 0;
	} else if (timer_num == 3){
		LPC_TIM3->TCR = 0;
	}
  return;
}

/*----------------------------------------------------------------------------
  Function that resets the specified timer
 *----------------------------------------------------------------------------*/
void reset_timer(uint8_t timer_num)
{
	uint32_t regVal;
	
	if (timer_num == 0){
		regVal = LPC_TIM0->TCR;
		regVal |= 0x02;
		LPC_TIM0->TCR = regVal;
  }
  else if (timer_num == 1){
		regVal = LPC_TIM1->TCR;
		regVal |= 0x02;
		LPC_TIM1->TCR = regVal;
  }
	else if (timer_num == 2){
		regVal = LPC_TIM2->TCR;
		regVal |= 0x02;
		LPC_TIM2->TCR = regVal;
	}
	else if (timer_num == 3){
		regVal = LPC_TIM3->TCR;
		regVal |= 0x02;
		LPC_TIM3->TCR = regVal;
	}
  	return;
}

/*----------------------------------------------------------------------------
  Function that initializes the specified timer with prescaler and match
 *----------------------------------------------------------------------------*/
uint32_t init_timer (uint8_t timer_num, uint32_t Prescaler, uint8_t MatchReg, uint8_t SRImatchReg, uint32_t TimerInterval)
{
	if (timer_num == 0){
		LPC_TIM0-> PR = Prescaler;
			
		if (MatchReg == 0){
			LPC_TIM0->MR0 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM0->MR1 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM0->MR2 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM0->MR3 = TimerInterval;
			LPC_TIM0->MCR |= SRImatchReg << 3*MatchReg;	
		}
		NVIC_EnableIRQ(TIMER0_IRQn);			/* enable timer interrupts    */
		NVIC_SetPriority(TIMER0_IRQn, 3);
		return (0);
	}
  else if (timer_num == 1){
		LPC_TIM1-> PR = Prescaler;
		
		if (MatchReg == 0){
			LPC_TIM1->MR0 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM1->MR1 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM1->MR2 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM1->MR3 = TimerInterval;
			LPC_TIM1->MCR |= SRImatchReg << 3*MatchReg;	
		}		
		NVIC_EnableIRQ(TIMER1_IRQn);
		NVIC_SetPriority(TIMER1_IRQn, 0);
		return (0);
  }
	else if (timer_num == 2){
		LPC_TIM2-> PR = Prescaler;
		
		if (MatchReg == 0){
			LPC_TIM2->MR0 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM2->MR1 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM2->MR2 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM2->MR3 = TimerInterval;
			LPC_TIM2->MCR |= SRImatchReg << 3*MatchReg;	
		}		
		NVIC_EnableIRQ(TIMER2_IRQn);
		NVIC_SetPriority(TIMER2_IRQn, 0);
		return (0);
  	}
	else if (timer_num == 3){
		LPC_TIM3-> PR = Prescaler;
		
		if (MatchReg == 0){
			LPC_TIM3->MR0 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 1){
			LPC_TIM3->MR1 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;			
		}
		else if (MatchReg == 2){
			LPC_TIM3->MR2 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;	
		}
		else if (MatchReg == 3){
			LPC_TIM3->MR3 = TimerInterval;
			LPC_TIM3->MCR |= SRImatchReg << 3*MatchReg;	
		}		
		NVIC_EnableIRQ(TIMER3_IRQn);
		NVIC_SetPriority(TIMER3_IRQn, 0);
		return (0);
  	}
	return (1);
}

/*----------------------------------------------------------------------------
  Function that returns the current counter value of the specified timer
 *----------------------------------------------------------------------------*/
unsigned int get_timer_value(uint8_t timer_num) {
	if (timer_num == 0){
		return LPC_TIM0->TC;
	}
	else if (timer_num == 1){
		return LPC_TIM1->TC;
	}
	else if (timer_num == 2){
		return LPC_TIM2->TC;
	}
	else if (timer_num == 3){
		return LPC_TIM3->TC;
	}
	return -1;
}

/*----------------------------------------------------------------------------
  Function that returns the current counter value of the specified timer in seconds
 *----------------------------------------------------------------------------*/
float get_timer_value_sec(uint8_t timer_num, uint32_t timer_freq) {
	switch (timer_num) {
		case 0:
			return (float) (LPC_TIM0->TC) / timer_freq;
    case 1:
			return (float) (LPC_TIM1->TC) / timer_freq;
    case 2:
			return (float) (LPC_TIM2->TC) / timer_freq;
    case 3:
			return (float) (LPC_TIM3->TC) / timer_freq;
    default:
			return -1; 
	}
}

/*----------------------------------------------------------------------------
  Function that checks if the specified timer is enabled
 *----------------------------------------------------------------------------*/
uint32_t is_timer_enabled (uint8_t timer_num){
	uint32_t regVal = 0;
	
	switch(timer_num){
		case 0:
			regVal = LPC_TIM0->TCR;
			break;
		case 1:
			regVal = LPC_TIM1->TCR;
			break;		
		case 2:
			regVal = LPC_TIM2->TCR;
			break;
		case 3:
			regVal = LPC_TIM3->TCR;
			break;
	}

	regVal &= 0x00000001;
	return regVal;
}

/*----------------------------------------------------------------------------
  Function that powers on Timer 2
 *----------------------------------------------------------------------------*/
void power_on_timer2(){
	LPC_SC -> PCONP |= (1 << 22);
}

/*----------------------------------------------------------------------------
  Function that powers on Timer 3
 *----------------------------------------------------------------------------*/
void power_on_timer3(){
	LPC_SC -> PCONP |= (1 << 23);
}