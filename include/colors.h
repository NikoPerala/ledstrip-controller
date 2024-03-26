#ifndef EPHO_COLORS_H
#define EPHO_COLORS_H

#include <stdint.h>
#include "light_ws2812.h"

#define COLOR_BLACK   (cRGB) { 0x00, 0x00, 0x00 }
#define COLOR_WHITE   (cRGB) { 0xff, 0xff, 0xff }
#define COLOR_RED     (cRGB) { 0xff, 0x00, 0x00 }
#define COLOR_GREEN   (cRGB) { 0x00, 0xff, 0x00 }
#define COLOR_BLUE    (cRGB) { 0x00, 0x00, 0xff }
#define COLOR_YELLOW  (cRGB) { 0xff, 0xff, 0x00 }
#define COLOR_CYAN    (cRGB) { 0x00, 0x00, 0xff }
#define COLOR_MAGENTA (cRGB) { 0xff, 0x00, 0xff }
#define COLOR_GRAY    (cRGB) { 0x80, 0x80, 0x80 }
#define COLOR_MAROON  (cRGB) { 0x80, 0x00, 0x00 }
#define COLOR_OLIVE   (cRGB) { 0x80, 0x80, 0x00 }
#define COLOR_PURPLE  (cRGB) { 0x80, 0x00, 0x80 }
#define COLOR_TEAL    (cRGB) { 0x00, 0x80, 0x80 }
#define COLOR_NAVY    (cRGB) { 0x00, 0x00, 0x80 }

#define COLOR_OFF COLOR_BLACK

typedef struct ColorRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} ColorRGB;

uint8_t lerp8(uint8_t a, uint8_t b, float pos);
cRGB lerp_cRGB(cRGB a, cRGB b, float pos);

uint8_t color_get_maximum(ColorRGB *color);
uint8_t color_get_minimum(ColorRGB *color);

cRGB HSV_to_ColorRGB(uint16_t hue, uint8_t sat, uint8_t val);

#endif // EPHO_COLOR_H
