#include "LPC17xx.h"
#include "led.h"

volatile unsigned char led_value;

/*----------------------------------------------------------------------------
  Function that initializes the LED GPIO pins
 *----------------------------------------------------------------------------*/
void LED_init(void) {
	LPC_PINCON->PINSEL4 &= 0xFFFF0000; //PIN mode GPIO (00b value per P2.0 to P2.7)
	LPC_GPIO2->FIODIR |= 0x000000FF; //P2.0...P2.7 Output (LEDs on PORT2 defined as Output)
  /* LPC_GPIO2->FIOSET = 0x000000FF; //all LEDs on */
	LPC_GPIO2->FIOCLR = 0x000000FF; //all LEDs off
}

/*----------------------------------------------------------------------------
  Function that deinitializes the LED GPIO pins
 *----------------------------------------------------------------------------*/
void LED_deinit(void) {
	LPC_GPIO2->FIODIR &= 0xFFFFFF00; //P2.0...P2.7 Output LEDs on PORT2 defined as Output
}
