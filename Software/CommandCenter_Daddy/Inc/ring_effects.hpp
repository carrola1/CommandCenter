#ifndef RING_EFFECTS_HPP
#define RING_EFFECTS_HPP

#include "dotstar.hpp"

struct RGB_VALS
{
   uint8_t r;
   uint8_t g;
   uint8_t b;
};

void ring_loop_animation(DotStar& ring, uint8_t n_loops, RGB_VALS rgb);
void ring_set_all_pixels(DotStar& ring, RGB_VALS rgb);


#endif // RING_EFFECTS_H
