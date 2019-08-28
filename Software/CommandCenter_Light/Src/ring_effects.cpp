#include "ring_effects.hpp"
#include "stm32l0xx_hal.h"

void ring_loop_animation(DotStar& ring, uint8_t n_loops, RGB_VALS rgb) {
    
    uint8_t ring_size = ring.numPixels();
    for (uint8_t i = 0; i < n_loops; i++) 
    {
        for(uint8_t j = ring_size; j > 0; j--)
        {
            ring.setPixelColor(j-1, rgb.r, rgb.g, rgb.b);
            if (j == ring_size) {
                ring.setPixelColor(0, 0, 0, 0);
            } else {
                ring.setPixelColor(j, 0, 0, 0);
            }
            ring.show();
            HAL_Delay(50);
        }
    }
    return;
}

void ring_set_all_pixels(DotStar& ring, RGB_VALS rgb) {
    uint8_t ring_size = ring.numPixels();
    for (uint8_t j = 0; j < ring_size; j++)
    {
        ring.setPixelColor(j, rgb.r, rgb.g, rgb.b);
    }
    ring.show();
}
