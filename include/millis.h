#ifndef MILLIS_JOINK_H
#define MILLIS_JOINK_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void millis_init();
uint32_t millis();

#endif
