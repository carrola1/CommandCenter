#include "motor.h"

motor_state update_motor(motor_state motor_position_last) {
  uint8_t motor_sw_state_0;
  uint8_t motor_sw_state_1;
  uint8_t motor_sw_state_2;
  uint8_t motor_sw_state;
  motor_sw_state_0 = HAL_GPIO_ReadPin(MOTOR_SW_0_GPIO_Port, MOTOR_SW_0_Pin);
  motor_sw_state_1 = HAL_GPIO_ReadPin(MOTOR_SW_1_GPIO_Port, MOTOR_SW_1_Pin);
  motor_sw_state_2 = HAL_GPIO_ReadPin(MOTOR_SW_2_GPIO_Port, MOTOR_SW_2_Pin);
  motor_sw_state = (motor_sw_state_2 << 2) + (motor_sw_state_1 << 1) + motor_sw_state_0;
  motor_state motor_position_new;
  switch(motor_sw_state) {
    case 6:
      motor_position_new = ST_left;
      break;
    case 5:
      motor_position_new = ST_center;
      break;
    case 3:
      motor_position_new = ST_right;
      break;
    default:
      motor_position_new = motor_position_last;
  }
  if (motor_position_new != motor_position_last){
    switch(motor_position_new) {
      case ST_left:
        HAL_LPTIM_PWM_Start(&hlptim1, PWM_PERIOD, PWM_LEFT_PULSE);
        break;
      case ST_center:
        HAL_LPTIM_PWM_Start(&hlptim1, PWM_PERIOD, PWM_CENTER_PULSE);
        break;
      case ST_right:
        HAL_LPTIM_PWM_Start(&hlptim1, PWM_PERIOD, PWM_RIGHT_PULSE);
        break;
    }
  } 
  return motor_position_new;
}

motor_state get_motor_position(void) {
  uint8_t motor_sw_state_0;
  uint8_t motor_sw_state_1;
  uint8_t motor_sw_state_2;
  uint8_t motor_sw_state;
  motor_sw_state_0 = HAL_GPIO_ReadPin(MOTOR_SW_0_GPIO_Port, MOTOR_SW_0_Pin);
  motor_sw_state_1 = HAL_GPIO_ReadPin(MOTOR_SW_1_GPIO_Port, MOTOR_SW_1_Pin);
  motor_sw_state_2 = HAL_GPIO_ReadPin(MOTOR_SW_2_GPIO_Port, MOTOR_SW_2_Pin);
  motor_sw_state = (motor_sw_state_2 << 2) + (motor_sw_state_1 << 1) + motor_sw_state_0;
  motor_state motor_position_new;
  switch(motor_sw_state) {
    case 6:
      motor_position_new = ST_left;
      break;
    case 5:
      motor_position_new = ST_center;
      break;
    case 3:
      motor_position_new = ST_right;
      break;
    default:
      motor_position_new = ST_center;
  }
  return motor_position_new;
}

void set_motor_position(motor_state motor_position) {
  switch(motor_position) {
    case ST_left:
      HAL_LPTIM_PWM_Start(&hlptim1, PWM_PERIOD, PWM_LEFT_PULSE);
      break;
    case ST_center:
      HAL_LPTIM_PWM_Start(&hlptim1, PWM_PERIOD, PWM_CENTER_PULSE);
      break;
    case ST_right:
      HAL_LPTIM_PWM_Start(&hlptim1, PWM_PERIOD, PWM_RIGHT_PULSE);
      break;
  }
}