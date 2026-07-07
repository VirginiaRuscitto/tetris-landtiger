#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include <stdint.h>

#define JOYSTICK_UP 29
#define JOYSTICK_DOWN 26
#define JOYSTICK_LEFT 27
#define JOYSTICK_RIGHT 28
#define JOYSTICK_CLICK 25

/* lib_joystick */
void joystick_init(void);
int joystick_dir(uint32_t dir);

#endif