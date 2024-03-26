#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"
#include "colors.h"

#define LED_AMOUNT 48
#define LEDSTRIP_STATES\
    X(STATE_RAINBOW)\
    X(STATE_STATIC_COLOR)

#define X(name) name,
typedef enum LedstripStates{
    LEDSTRIP_STATES
} Ledstrip_states;
#undef X

#define COLOR_AMOUNT 13
const cRGB colors[COLOR_AMOUNT] = {
    COLOR_WHITE,  
    COLOR_RED,    
    COLOR_GREEN,  
    COLOR_BLUE,   
    COLOR_YELLOW, 
    COLOR_CYAN,   
    COLOR_MAGENTA,
    COLOR_GRAY,   
    COLOR_MAROON, 
    COLOR_OLIVE,  
    COLOR_PURPLE, 
    COLOR_TEAL,   
    COLOR_NAVY   
};

void set_all_to_cRGB(cRGB leds[LED_AMOUNT], cRGB color)
{
    for (int i = 0; i < LED_AMOUNT; ++i){
        leds[i] = color;
    }
}

typedef struct LedController {
    uint8_t state;
    float angle;
    uint8_t position;
    uint8_t color_count;
    uint8_t send_data : 1;
} LedController;

typedef struct Button {
    uint8_t state;
    uint8_t previous_state;
} Button;

int main(void)
{

    DDRD  &=  ~(1 << PORTD2);   // Pin 2 as input
    PORTD |= (1 << PORTD2);   // Set pullup resistor

    cRGB leds[LED_AMOUNT];

    LedController lc = { 
        .state = STATE_STATIC_COLOR, 
        .angle = 0.f, 
        .position = 0, 
        .color_count = 0,
        .send_data = 0
    };

    Button button = { .state = 0, .previous_state = 0 };

    uint8_t state = STATE_STATIC_COLOR;

    for(;;)
    {
        
        button.state = !(PIND & (1 << PORTD2));
        if (button.state && !button.previous_state){
            lc.color_count++;
            if (lc.color_count >= COLOR_AMOUNT) lc.color_count = 0;
        }
        button.previous_state = button.state;

        switch (lc.state){        
            case STATE_STATIC_COLOR:
                set_all_to_cRGB(leds, colors[lc.color_count]);
                lc.send_data = 1;
                break;
        }

        if (lc.send_data) {
            ws2812_setleds(leds, LED_AMOUNT);
            lc.send_data = 0;
        }

    }


  return 0;

}
