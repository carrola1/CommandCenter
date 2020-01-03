#include "bar_graph.hpp"
#include "stm32l0xx_hal.h"
#include "spi.h"
#include "math.h"

// Constructor
BarGraph::BarGraph(uint8_t num_switch, uint8_t num_led) {
  max_switch = num_switch;
  max_led = num_led;
  incr = num_led/num_switch;
}

void BarGraph::begin() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

}

void BarGraph::update(uint8_t *sw_state) {
  uint8_t cur_switch = 0;
  for (uint8_t i=0; i<max_switch; i++) {
    if (sw_state[i] == 1) {
      cur_switch = i;
      break;
    }
  }
  if (cur_switch != last_switch) {
    last_switch = cur_switch;
    uint8_t num_led = incr*(cur_switch+1);
    lc.clearDisplay(0);
    for (uint8_t i=0; i<num_led; i++) {
      lc.setLed(0,i%8, floor((i/8)%8), true);
    }
  }
}

void BarGraph::clear_display() {
  lc.clearDisplay(0);
  last_switch = 8;
}