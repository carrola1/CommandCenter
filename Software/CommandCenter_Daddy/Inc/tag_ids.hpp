#ifndef _TAG_IDS_HPP_
#define _TAG_IDS_HPP_

#include <stdint.h>
#include <string.h>
#include "ring_effects.hpp"

#define num_tags 4

struct tag
{
     uint8_t uid[7];
     RGB_VALS rgb;
     char name[32];
     char wav_file_find[32];
     char wav_file_found[32];
     char wav_file_color[32];
};

const tag tag0 = {{165,175,87,201,0,0,0}, {220,80,5}, "orange", "find_orange_star.wav", "found_orange_star.wav", "orange.wav"};
const tag tag1 = {{37,176,87,201,0,0,0}, {220,0,0}, "red", "find_red_triangle.wav", "found_red_triangle.wav", "red.wav"};
const tag tag2 = {{133,196,84,106,0,0,0}, {0,220,0}, "green", "find_green_heart.wav", "found_green_heart.wav", "green.wav"};     //green heart
const tag tag3 = {{69,55,85,106,0,0,0}, {0,0,220}, "blue", "find_blue_square.wav", "found_blue_square.wav", "blue.wav"};         //blue square
const struct tag tags[num_tags] = {tag0, tag1, tag2, tag3};

struct tag find_tag(uint8_t * uid);

#endif
