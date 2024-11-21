#ifndef TIMER_H
#define TIMER_H

#include "globals.h"
#include <stdint.h>

uint8_t set_delay_timer(System *sys, uint8_t val);
uint8_t set_sound_timer(System *sys, uint8_t val);
uint8_t read_delay_timer(System *sys);

#endif // TIMER_H
