#ifndef __LIGHT_CONTROL_FUNCS_H
#define __LIGHT_CONTROL_FUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dotstar.hpp"
#include "gpio.h"
#include "bar_graph.hpp"
#include "main.h"

extern uint8_t bg_sw_states[8];
extern GPIO_PinState tog_sw_states[3];
extern GPIO_PinState push_sw_state;

// Color State Machine
extern color_states color_state;

#define bg_debounce 4

void read_tog_sw_states(void);
uint8_t check_tog_sw_states(void);
void update_tog_sw_states(void);
void read_bg_sw_states(void);
uint8_t check_bg_sw_states(void);
void update_bg(BarGraph& bg);
void read_push_sw_state(void);
uint8_t check_push_sw_state(void);
void update_push_sw_state(uint8_t * ring_dir);
RGB_VALS rgb_lut(uint8_t color);
void set_all_pixels(DotStar& ring, RGB_VALS rgb);
void turn_all_leds_off(DotStar& ring, BarGraph& bg);
void send_audio(uint8_t audio_trig_index);

#ifdef __cplusplus
}
#endif

#endif