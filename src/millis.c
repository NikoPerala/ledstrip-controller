#include "millis.h"

volatile uint32_t milliseconds = 0;

void millis_init() {
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = 1999;
    TCCR1B |= (1 << CS11);
}

ISR(TIMER1_COMPA_vect){
    milliseconds++;
}

uint32_t millis() {
    uint32_t ms;

    cli();
    ms = milliseconds;
    sei();
    return ms;
}
