#ifndef MOTOR_H
#define MOTOR_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "stm32l0xx_hal.h"
#include "lptim.h"
#include "gpio.h"

#define PWM_PERIOD 0x1388
#define PWM_LEFT_PULSE 0x0258
#define PWM_CENTER_PULSE 0x0190
#define PWM_RIGHT_PULSE 0x00AF

typedef enum motor_state {ST_left, ST_center, ST_right} motor_state;

motor_state update_motor(motor_state motor_position_last);
motor_state get_motor_position(void);
void set_motor_position(motor_state motor_position);

#ifdef __cplusplus
}
#endif
#endif