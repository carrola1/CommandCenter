#ifndef BAR_GRAPH_HPP
#define BAR_GRAPH_HPP

#include <stdint.h>
#include "LedControl.h"

class BarGraph {
    private :
      uint8_t max_switch;
      uint8_t max_led;
      uint8_t last_switch = 0;
      uint8_t incr;
      LedControl lc = LedControl(1);
    public :
      BarGraph(uint8_t num_switch, uint8_t num_led);
      void begin();
      void update(uint8_t *sw_state);
};

#endif  //bar_graph.hpp







