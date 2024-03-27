#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "millis.h"
#include "light_ws2812.h"
#include "colors.h"
#include "button.h"

#define LED_AMOUNT 48
#define RAINBOW_STEP 65535.0 / LED_AMOUNT



#define LEDSTRIP_STATES\
    X(STATE_RAINBOW)\
    X(STATE_STATIC_COLOR)\
    X(STATE_OFF)\
    X(STATE_COUNT)

#define X(name) name,
typedef enum LedstripStates{
    LEDSTRIP_STATES
} Ledstrip_states;
#undef X

#define COLOR_AMOUNT 13
const cRGB colors[COLOR_AMOUNT] = {
    COLOR_RED,    
    COLOR_WHITE,  
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
    uint8_t interval;
    uint8_t send_data : 1;
    uint8_t set_data : 1;
    Button button;
} LedController;

typedef struct PinParams {
    volatile uint8_t *port;
    uint8_t pin;
} PinParams;

uint8_t read_pin(Button *button)
{
    PinParams *params = (PinParams*) button->read_params;
    return (*params->port & (1 << params->pin));
}

uint8_t read_pin_invert(Button *button)
{
    return !read_pin(button);
}

int main(void)
{

    DDRD  &=  ~(1 << PORTD2);   // Pin 2 as input
    PORTD |=   (1 << PORTD2);   // Set pullup resistor

    millis_init();

    cRGB leds[LED_AMOUNT];

    PinParams button_params = { .port = &PIND, .pin = 2 };
    Button button = { 
        .press = BUTTON_PRESS_NONE, 
        .previous_state = 0, 
        .press_time = 0, 
        .read_params = &button_params,
        .read_func = read_pin_invert
    };

    LedController lc = { 
        .state = STATE_STATIC_COLOR, 
        .angle = 0.f, 
        .position = 0, 
        .color_count = 0,
        .interval = 17,
        .send_data = 0,
        .set_data = 1,
        .button = button
    };

    uint32_t c_time = millis();
    uint32_t p_time = c_time;


    int button_pressed = 0; 
    int button_press_type = 0;

    int rainbow_offset = 0;

    for(;;)
    {
        c_time = millis();

        button_pressed = read_button(&lc.button, c_time);
        if (button_pressed){
            button_press_type = get_button_press_type(&lc.button);
            if (button_press_type == BUTTON_PRESS_LONG){
                lc.set_data = 1;
                lc.state++;
                if (lc.state >= STATE_COUNT - 1) lc.state = 0;
            }
        }

        switch (lc.state){        
            case STATE_STATIC_COLOR:
                if ((button_pressed && button_press_type == BUTTON_PRESS_QUICK) || lc.set_data){
                    lc.color_count++;
                    if (lc.color_count == COLOR_AMOUNT) lc.color_count = 0;
                    set_all_to_cRGB(leds, colors[lc.color_count]);
                    lc.send_data = 1;
                    lc.set_data = 0;
                }
                break;
            case STATE_RAINBOW:
                for (int i = 0; i < LED_AMOUNT; ++i){
                    leds[i] = HSV_to_ColorRGB(i * RAINBOW_STEP, 0xff, 0xff);
                }
                lc.send_data = 1;
                lc.set_data = 0;
                break;
            case STATE_OFF:
                set_all_to_cRGB(leds, (cRGB) { 0, 0, 0});
                lc.send_data = 1;
                break;
        }
       
        if (button_pressed && button_press_type == BUTTON_PRESS_DOUBLE) lc.state = STATE_OFF;

        if (lc.send_data) {
            ws2812_setleds(leds, LED_AMOUNT);
            lc.send_data = 0;
        }

    }
  return 0;
}
