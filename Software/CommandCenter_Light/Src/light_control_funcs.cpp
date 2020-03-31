#include "light_control_funcs.h"
#include "stm32l0xx_hal.h"
#include "rng.h"
#include "APDS9960.h"
#include "usart.h"
#include "ring_effects.hpp"

void read_tog_sw_states(void) {
  tog_sw_states[0] = HAL_GPIO_ReadPin(SW_0_GPIO_Port, SW_0_Pin);
  tog_sw_states[1] = HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
  tog_sw_states[2] = HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);
}

uint8_t check_tog_sw_states(void) {
  GPIO_PinState tog_sw_states_new[3];
  tog_sw_states_new[0] = HAL_GPIO_ReadPin(SW_0_GPIO_Port, SW_0_Pin);
  tog_sw_states_new[1] = HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
  tog_sw_states_new[2] = HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);

  for (uint8_t sw_i = 0; sw_i < 3; sw_i++) {
    if (tog_sw_states_new[sw_i] != tog_sw_states[sw_i]) {
      return 1;
    }
  }
  return 0;
}

void update_tog_sw_states(void) {
  GPIO_PinState tog_sw_states_new[3];
  tog_sw_states_new[0] = HAL_GPIO_ReadPin(SW_0_GPIO_Port, SW_0_Pin);
  tog_sw_states_new[1] = HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
  tog_sw_states_new[2] = HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);

  HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, tog_sw_states_new[0]);
  HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, tog_sw_states_new[1]);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, tog_sw_states_new[2]);

  for (uint8_t sw_i = 0; sw_i < 3; sw_i++) {
    if ((tog_sw_states_new[sw_i] == GPIO_PIN_SET) && (tog_sw_states[sw_i] == GPIO_PIN_RESET)) {
      send_audio(sw_i);
    }
    tog_sw_states[sw_i] = tog_sw_states_new[sw_i];
  }
}

static uint8_t bg_sw_states_last[8];
void read_bg_sw_states(void) {
  bg_sw_states[0] = !HAL_GPIO_ReadPin(BG_SW_0_GPIO_Port, BG_SW_0_Pin);
  bg_sw_states[1] = !HAL_GPIO_ReadPin(BG_SW_1_GPIO_Port, BG_SW_1_Pin);
  bg_sw_states[2] = !HAL_GPIO_ReadPin(BG_SW_2_GPIO_Port, BG_SW_2_Pin);
  bg_sw_states[3] = !HAL_GPIO_ReadPin(BG_SW_3_GPIO_Port, BG_SW_3_Pin);
  bg_sw_states[4] = !HAL_GPIO_ReadPin(BG_SW_4_GPIO_Port, BG_SW_4_Pin);
  bg_sw_states[5] = !HAL_GPIO_ReadPin(BG_SW_5_GPIO_Port, BG_SW_5_Pin);
  bg_sw_states[6] = !HAL_GPIO_ReadPin(BG_SW_6_GPIO_Port, BG_SW_6_Pin);
  bg_sw_states[7] = !HAL_GPIO_ReadPin(BG_SW_7_GPIO_Port, BG_SW_7_Pin);

  for (uint8_t i = 0; i < 8; i++) {
    bg_sw_states_last[i] = bg_sw_states[i];
  }

}

uint8_t check_bg_sw_states(void) {
  uint8_t bg_sw_states_new[8];
  bg_sw_states_new[0] = !HAL_GPIO_ReadPin(BG_SW_0_GPIO_Port, BG_SW_0_Pin);
  bg_sw_states_new[1] = !HAL_GPIO_ReadPin(BG_SW_1_GPIO_Port, BG_SW_1_Pin);
  bg_sw_states_new[2] = !HAL_GPIO_ReadPin(BG_SW_2_GPIO_Port, BG_SW_2_Pin);
  bg_sw_states_new[3] = !HAL_GPIO_ReadPin(BG_SW_3_GPIO_Port, BG_SW_3_Pin);
  bg_sw_states_new[4] = !HAL_GPIO_ReadPin(BG_SW_4_GPIO_Port, BG_SW_4_Pin);
  bg_sw_states_new[5] = !HAL_GPIO_ReadPin(BG_SW_5_GPIO_Port, BG_SW_5_Pin);
  bg_sw_states_new[6] = !HAL_GPIO_ReadPin(BG_SW_6_GPIO_Port, BG_SW_6_Pin);
  bg_sw_states_new[7] = !HAL_GPIO_ReadPin(BG_SW_7_GPIO_Port, BG_SW_7_Pin);

  for (uint8_t i = 0; i < 8; i++) {
    if (bg_sw_states_new[i] != bg_sw_states[i]) {
      return 1;
    }
  }

  return 0;
}

static uint8_t bg_change_det_cnt = bg_debounce;
static uint8_t bg_change_det = 1;

void update_bg(BarGraph& bg) {
  uint8_t bg_sw_states_new[8];
  bg_sw_states_new[0] = !HAL_GPIO_ReadPin(BG_SW_0_GPIO_Port, BG_SW_0_Pin);
  bg_sw_states_new[1] = !HAL_GPIO_ReadPin(BG_SW_1_GPIO_Port, BG_SW_1_Pin);
  bg_sw_states_new[2] = !HAL_GPIO_ReadPin(BG_SW_2_GPIO_Port, BG_SW_2_Pin);
  bg_sw_states_new[3] = !HAL_GPIO_ReadPin(BG_SW_3_GPIO_Port, BG_SW_3_Pin);
  bg_sw_states_new[4] = !HAL_GPIO_ReadPin(BG_SW_4_GPIO_Port, BG_SW_4_Pin);
  bg_sw_states_new[5] = !HAL_GPIO_ReadPin(BG_SW_5_GPIO_Port, BG_SW_5_Pin);
  bg_sw_states_new[6] = !HAL_GPIO_ReadPin(BG_SW_6_GPIO_Port, BG_SW_6_Pin);
  bg_sw_states_new[7] = !HAL_GPIO_ReadPin(BG_SW_7_GPIO_Port, BG_SW_7_Pin);

  for (uint8_t i = 0; i < 8; i++) {
    if (bg_sw_states_new[i] != bg_sw_states[i]) {
      bg_change_det_cnt = 0;
      bg_change_det = 1;
      break;
    }
  }

  if (bg_change_det_cnt == bg_debounce) {
    if (bg_change_det) {
      bg_change_det = 0;
      bg.update(&bg_sw_states_new[0]);
      for (uint8_t i = 0; i < 8; i++) {
        if (bg_sw_states_new[i] != bg_sw_states_last[i]) {
          if (bg_sw_states_new[i] > bg_sw_states_last[i]) {
            send_audio(AUDIO_TRIG_BG_DOWN);
          } else {
            send_audio(AUDIO_TRIG_BG_UP);
          }
        }
        bg_sw_states_last[i] = bg_sw_states[i]; 
      }
    }
  } else {
    bg_change_det_cnt++;
  }

  for (uint8_t i = 0; i < 8; i++) {
    bg_sw_states[i] = bg_sw_states_new[i];
  }

}

void read_push_sw_state(void) {
  push_sw_state = HAL_GPIO_ReadPin(SW_3_GPIO_Port, SW_3_Pin);
}

uint8_t check_push_sw_state(void) {
  GPIO_PinState push_sw_state_new = HAL_GPIO_ReadPin(SW_3_GPIO_Port, SW_3_Pin);
  if (push_sw_state_new != push_sw_state) {
    return 1;
  } else {
    return 0;
  }
}

void update_push_sw_state(uint8_t * ring_dir) {
  GPIO_PinState push_sw_state_new = HAL_GPIO_ReadPin(SW_3_GPIO_Port, SW_3_Pin);
  uint8_t ring_dir_new;
  if ((push_sw_state_new == GPIO_PIN_SET) && (push_sw_state == GPIO_PIN_RESET)) {
    ring_dir_new = !(*ring_dir);
    send_audio(AUDIO_TRIG_SW3);
  } else {
    ring_dir_new = *ring_dir;
  }
  push_sw_state = push_sw_state_new;
  *ring_dir = ring_dir_new;
}

RGB_VALS rgb_lut(uint8_t color) {
  RGB_VALS rgb;
  if (color == RED) {
    rgb = RED_RGB;
  } else if (color == ORANGE) {
    rgb = ORANGE_RGB;
  } else if (color == BLUE) {
    rgb = BLUE_RGB;
  } else if (color == GREEN) {
    rgb = GREEN_RGB;
  } else if (color == PINK) {
    rgb = PINK_RGB;
  } else if (color == PURPLE) {
    rgb = PURPLE_RGB;
  } else if (color == YELLOW) {
    rgb = YELLOW_RGB;
  }
  return rgb;
}

void set_all_pixels(DotStar& ring, RGB_VALS rgb) {
  ring.setBrightness(100);
  ring_set_all_pixels(ring, rgb);
  ring.show();
  ring.setBrightness(200);
}

void turn_all_leds_off(DotStar& ring, BarGraph& bg) {
  RGB_VALS rgb = {0, 0, 0};
  ring_set_all_pixels(ring, rgb);
  bg.clear_display();
  HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
}

void send_audio(uint8_t audio_trig_index) {
  if ((color_state != ST_color_wait_for_song) && (color_state != ST_color_wait_for_request)) {
    HAL_UART_Transmit(&huart1, &audio_trig_index, 1, HAL_MAX_DELAY);
  }
}