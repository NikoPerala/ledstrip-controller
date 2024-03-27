#ifndef EPHO_PUSH_BUTTON_H
#define EPHO_PUSH_BUTTON_H

#include <stdint.h>

#define BUTTON_LONG_PRESS_INTERVAL 500
#define QUICK_PUSH_MIN_INTERVAL 30

typedef enum {
    BUTTON_PRESS_NONE,
    BUTTON_PRESS_BENDING,
    BUTTON_PRESS_QUICK,
    BUTTON_PRESS_LONG
} ButtonPressType;

typedef struct Button Button;
struct Button {
    ButtonPressType press;
    uint8_t previous_state;
    uint32_t press_time;
    void *read_params;
    uint8_t (*read_func)(Button *button);
};

uint8_t read_button(Button *button, uint32_t c_time);
uint8_t get_button_press_type(Button *button);

#endif // EPHO_PUSH_BUTTON_H
