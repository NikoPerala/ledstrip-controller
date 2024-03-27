#include "button.h"

uint8_t read_button(Button *button, uint32_t c_time)
{
    uint8_t button_state = button->read_func(button);

    if (button_state && !button->previous_state){
        button->press = BUTTON_PRESS_BENDING;
        button->press_time = c_time;
    } else if (button->previous_state && !button_state && button->press == BUTTON_PRESS_BENDING){
        button->press = BUTTON_PRESS_QUICK;
        return 1;
    } else if (button_state && c_time - button->press_time >= BUTTON_LONG_PRESS_INTERVAL){
        button->press = BUTTON_PRESS_LONG;
        return 1;
    }
    button->previous_state = button_state;

    return 0;
}

uint8_t get_button_press_type(Button *button)
{
    ButtonPressType ret = button->press;
    button->press = BUTTON_PRESS_NONE;
    return ret;
}
