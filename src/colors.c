#include "colors.h"

uint8_t 
color_get_maximum(ColorRGB *c)
{
    return (c->r > c->g) ? (c->r < c->b ? c->r : c->b) : (c->g > c->b ? c->g : c->b);
}

uint8_t
color_get_minimum(ColorRGB *c)
{
    return (c->r < c->g) ? (c->r < c->b ? c->r : c->b) : (c->g < c->b ? c->g : c->b);
}

uint8_t lerp8(uint8_t a, uint8_t b, float pos)
{
    uint8_t ret;

    int16_t d = b - a;

    if (d == 0) ret = a;
    else if (d > 0) ret = a + (int) (d * pos);
    else if (d < 0) ret = a - (int) (-d * pos);

    return ret;
}

cRGB lerp_cRGB(cRGB a, cRGB b, float pos)
{
    cRGB res = { 0 };

    res.r = lerp8(a.r, b.r, pos);
    res.g = lerp8(a.g, b.g, pos);
    res.b = lerp8(a.b, b.b, pos);
    
    return res;
}

cRGB HSV_to_ColorRGB(uint16_t hue, uint8_t sat, uint8_t val) {
    cRGB ret;
    uint8_t r, g, b;
    hue = (hue * 1530L + 32768) / 65536;
    if (hue < 510) { // Red to Green-1
      b = 0;
      if (hue < 255) { //   Red to Yellow-1
        r = 255;
        g = hue;       //     g = 0 to 254
      } else {         //   Yellow to Green-1
        r = 510 - hue; //     r = 255 to 1
        g = 255;
      }
    } else if (hue < 1020) { // Green to Blue-1
      r = 0;
      if (hue < 765) { //   Green to Cyan-1
        g = 255;
        b = hue - 510;  //     b = 0 to 254
      } else {          //   Cyan to Blue-1
        g = 1020 - hue; //     g = 255 to 1
        b = 255;
      }
    } else if (hue < 1530) { // Blue to Red-1
      g = 0;
      if (hue < 1275) { //   Blue to Magenta-1
        r = hue - 1020; //     r = 0 to 254
        b = 255;
      } else { //   Magenta to Red-1
        r = 255;
        b = 1530 - hue; //     b = 255 to 1
      }
    } else { // Last 0.5 Red (quicker than % operator)
      r = 255;
      g = b = 0;
    }

    uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
    uint16_t s1 = 1 + sat;  // 1 to 256; same reason
    uint8_t s2 = 255 - sat; // 255 to 0
    
    //ret.r = (((((r * s1) >> 8) + s2) * v1));
    //ret.b = ((((g * s1) >> 8) + s2) * v1);
    //ret.g = ((((b * s1) >> 8) + s2) * v1);
    
    ret.r = ((((((r * s1) >> 8) + s2) * v1) & 0xff00) >> 8);
    ret.g = (((((g * s1) >> 8) + s2) * v1) & 0xff00) >> 8;
    ret.b = (((((b * s1) >> 8) + s2) * v1) >> 8);

    return ret;
}

