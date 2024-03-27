#include "button.h"

uint8_t read_button(Button *button, uint32_t c_time)
{
    uint8_t button_state = button->read_func(button);
    uint8_t retval = 0;

    if (button_state && !button->previous_state) { 
        if (button->press == BUTTON_PRESS_NONE){
            button->press = BUTTON_PRESS_BENDING;
            button->press_time = c_time;
        } else if (button->press == BUTTON_PRESS_BENDING_QUICK){
            retval = 1;
            button->press = BUTTON_PRESS_DOUBLE;
        }

    } 
    
    else if (button->previous_state && !button_state && button->press == BUTTON_PRESS_BENDING){
        button->press = BUTTON_PRESS_BENDING_QUICK;
    } 

    else if (button->press == BUTTON_PRESS_BENDING_QUICK && c_time - button->press_time >= QUICK_PUSH_MIN_INTERVAL){
        button->press = BUTTON_PRESS_QUICK;
        retval = 1;
    }

    else if (button_state && c_time - button->press_time >= BUTTON_LONG_PRESS_INTERVAL && button->press == BUTTON_PRESS_BENDING){
        button->press = BUTTON_PRESS_LONG;
        retval = 1;
    }
    button->previous_state = button_state;

    return retval;
}

uint8_t get_button_press_type(Button *button)
{
    ButtonPressType ret = button->press;
    button->press = BUTTON_PRESS_NONE;
    return ret;
}
