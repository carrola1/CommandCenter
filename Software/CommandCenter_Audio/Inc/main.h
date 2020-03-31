/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AUDIO_SD_N_0_Pin GPIO_PIN_13
#define AUDIO_SD_N_0_GPIO_Port GPIOC
#define AUDIO_SD_N_1_Pin GPIO_PIN_15
#define AUDIO_SD_N_1_GPIO_Port GPIOC
#define SD_DETECT_Pin GPIO_PIN_2
#define SD_DETECT_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOA
#define SD_SW_Pin GPIO_PIN_3
#define SD_SW_GPIO_Port GPIOA
#define WKUP_Pin GPIO_PIN_0
#define WKUP_GPIO_Port GPIOA
#define MOTOR_SW_0_Pin GPIO_PIN_0
#define MOTOR_SW_0_GPIO_Port GPIOB
#define MOTOR_SW_1_Pin GPIO_PIN_1
#define MOTOR_SW_1_GPIO_Port GPIOB
#define MOTOR_CTRL_Pin GPIO_PIN_2
#define MOTOR_CTRL_GPIO_Port GPIOB
#define TEST_LED_Pin GPIO_PIN_11
#define TEST_LED_GPIO_Port GPIOA
#define PWR_ENB_Pin GPIO_PIN_3
#define PWR_ENB_GPIO_Port GPIOB
#define MOTOR_SW_2_Pin GPIO_PIN_8
#define MOTOR_SW_2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define AUDIO_TRIG_SW0          0
#define AUDIO_TRIG_SW1          1
#define AUDIO_TRIG_SW2          2
#define AUDIO_TRIG_SW3          3
#define AUDIO_TRIG_BG_UP        4
#define AUDIO_TRIG_BG_DOWN      5
#define AUDIO_TRIG_START        6
#define AUDIO_TRIG_FIND_RED     7
#define AUDIO_TRIG_FIND_GREEN   8
#define AUDIO_TRIG_FIND_BLUE    9
#define AUDIO_TRIG_FIND_YELLOW  10
#define AUDIO_TRIG_FIND_PURPLE  11
#define AUDIO_TRIG_FIND_PINK    12
#define AUDIO_TRIG_FIND_ORANGE  13
#define AUDIO_TRIG_FOUND_RED    14
#define AUDIO_TRIG_FOUND_GREEN  15
#define AUDIO_TRIG_FOUND_BLUE   16
#define AUDIO_TRIG_FOUND_YELLOW 17
#define AUDIO_TRIG_FOUND_PURPLE 18
#define AUDIO_TRIG_FOUND_PINK   19
#define AUDIO_TRIG_FOUND_ORANGE 20
#define AUDIO_TRIG_MISS_RED     21
#define AUDIO_TRIG_MISS_GREEN   22
#define AUDIO_TRIG_MISS_BLUE    23
#define AUDIO_TRIG_MISS_YELLOW  24
#define AUDIO_TRIG_MISS_PURPLE  25
#define AUDIO_TRIG_MISS_PINK    26
#define AUDIO_TRIG_MISS_ORANGE  27
#define AUDIO_TRIG_PETE         28
#define AUDIO_TRIG_SNUGGLE_PUP  29
#define AUDIO_TRIG_ELMO         30
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
