#ifndef _CONST_H_
#define _CONST_H_

#include <stdint.h>

// Define some constant to share accross all project.

// Define timers
// see: https://freemint.github.io/tos.hyp/en/xbios_datetime.html#Xbtimer
#define TIMER_A 0      // usable for own programs
#define TIMER_HBLANK 1 // Horizontal blank interrupt
#define TIMER_SYSTEM 2 // 200Hz system timer
#define TIMER_BAUD 3   // Generator for the Baud rates

#define INTERRUPTION_SERVICE_ADDRESS (uint8_t *)0xFFFFFA0FL

const uint8_t END_OF_INTERRUPT_TIMER_A = ~(1 << 5);
const uint8_t END_OF_INTERRUPT_TIMER_HBLANK = ~(1 << 0);

#endif // _CONST_H_
