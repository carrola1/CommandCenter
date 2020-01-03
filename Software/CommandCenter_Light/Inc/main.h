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
#define LED_0_Pin GPIO_PIN_1
#define LED_0_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_2
#define LED_1_GPIO_Port GPIOA
#define SPI_SS_Pin GPIO_PIN_4
#define SPI_SS_GPIO_Port GPIOA
#define DOTSTAR_DATA_Pin GPIO_PIN_0
#define DOTSTAR_DATA_GPIO_Port GPIOB
#define DOTSTAR_CLK_Pin GPIO_PIN_1
#define DOTSTAR_CLK_GPIO_Port GPIOB
#define BG_SW_0_Pin GPIO_PIN_2
#define BG_SW_0_GPIO_Port GPIOB
#define BG_SW_6_Pin GPIO_PIN_10
#define BG_SW_6_GPIO_Port GPIOB
#define BG_SW_7_Pin GPIO_PIN_11
#define BG_SW_7_GPIO_Port GPIOB
#define SW_0_Pin GPIO_PIN_12
#define SW_0_GPIO_Port GPIOB
#define SW_1_Pin GPIO_PIN_13
#define SW_1_GPIO_Port GPIOB
#define SW_2_Pin GPIO_PIN_14
#define SW_2_GPIO_Port GPIOB
#define SW_3_Pin GPIO_PIN_15
#define SW_3_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_8
#define LED_2_GPIO_Port GPIOA
#define TEST_LED_Pin GPIO_PIN_11
#define TEST_LED_GPIO_Port GPIOA
#define LED_3_Pin GPIO_PIN_12
#define LED_3_GPIO_Port GPIOA
#define COLOR_INT_Pin GPIO_PIN_15
#define COLOR_INT_GPIO_Port GPIOA
#define BG_SW_1_Pin GPIO_PIN_3
#define BG_SW_1_GPIO_Port GPIOB
#define BG_SW_2_Pin GPIO_PIN_4
#define BG_SW_2_GPIO_Port GPIOB
#define BG_SW_3_Pin GPIO_PIN_5
#define BG_SW_3_GPIO_Port GPIOB
#define BG_SW_4_Pin GPIO_PIN_8
#define BG_SW_4_GPIO_Port GPIOB
#define BG_SW_5_Pin GPIO_PIN_9
#define BG_SW_5_GPIO_Port GPIOB
#define WKUP_Pin GPIO_PIN_0
#define WKUP_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define AUDIO_TRIG_SW0          0
#define AUDIO_TRIG_SW1          1
#define AUDIO_TRIG_SW2          2
#define AUDIO_TRIG_SW3          3
#define AUDIO_TRIG_BG           4
#define AUDIO_TRIG_FIND_RED     5
#define AUDIO_TRIG_FIND_GREEN   6
#define AUDIO_TRIG_FIND_BLUE    7
#define AUDIO_TRIG_FIND_YELLOW  8
#define AUDIO_TRIG_FIND_PURPLE  9
#define AUDIO_TRIG_FIND_PINK    10
#define AUDIO_TRIG_FIND_ORANGE  11
#define AUDIO_TRIG_FOUND_RED    12
#define AUDIO_TRIG_FOUND_GREEN  13
#define AUDIO_TRIG_FOUND_BLUE   14
#define AUDIO_TRIG_FOUND_YELLOW 15
#define AUDIO_TRIG_FOUND_PURPLE 16
#define AUDIO_TRIG_FOUND_PINK   17
#define AUDIO_TRIG_FOUND_ORANGE 18
#define AUDIO_TRIG_MISS_RED     19
#define AUDIO_TRIG_MISS_GREEN   20
#define AUDIO_TRIG_MISS_BLUE    21
#define AUDIO_TRIG_MISS_YELLOW  22
#define AUDIO_TRIG_MISS_PURPLE  23
#define AUDIO_TRIG_MISS_PINK    24
#define AUDIO_TRIG_MISS_ORANGE  25
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
