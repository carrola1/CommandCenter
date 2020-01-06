
/**
  ******************************************************************************
  * @file           : mainc
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners
  *
  * Copyright (c) 2018 STMicroelectronics International NV
  * All rights reserved
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1 Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer
  * 2 Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution
  * 3 Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission
  * 4 This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics
  * 5 Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "rng.h"
#include "dotstar.hpp"
#include "ring_effects.hpp"
#include "bar_graph.hpp"
#include "APDS9960.h"
#include "usart.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t uart_data;
uint8_t uart_ready = 1;
uint8_t audio_playing = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  void send_audio(uint8_t audio_trig_index);
  GPIO_PinState * read_sw_states(void);
  uint8_t * read_bg_states(void);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_RNG_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(200);

  // Remember switch states
  GPIO_PinState sw_states_old[4];
  sw_states_old[0] = HAL_GPIO_ReadPin(SW_0_GPIO_Port, SW_0_Pin);
  sw_states_old[1] = HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
  sw_states_old[2] = HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);
  sw_states_old[3] = HAL_GPIO_ReadPin(SW_3_GPIO_Port, SW_3_Pin);
  GPIO_PinState* sw_states_new;
  
  // Setup and initialize Dotstars
  DotStar ring = DotStar(20, DOTSTAR_BGR);
  ring.begin(); // Initialize pins for output
  RGB_VALS rgb_off;
  rgb_off.r = 0; rgb_off.g = 0; rgb_off.b = 0;
  ring_set_all_pixels(ring, rgb_off); // Initialize LEDs to off
  RGB_VALS rgb_default; 
  RGB_VALS rgb_new;
  rgb_default.r = 20; rgb_default.g = 10; rgb_default.b = 180;
  ring.setBrightness(200);
  uint8_t ring_dir = 0;

  // Initialize LED driver
  BarGraph bg=BarGraph(8,40);
  bg.begin();
  uint8_t bg_sw_old[8];
  uint8_t* bg_sw_new;
  bg_sw_old[0] = !HAL_GPIO_ReadPin(BG_SW_0_GPIO_Port, BG_SW_0_Pin);
  bg_sw_old[1] = !HAL_GPIO_ReadPin(BG_SW_1_GPIO_Port, BG_SW_1_Pin);
  bg_sw_old[2] = !HAL_GPIO_ReadPin(BG_SW_2_GPIO_Port, BG_SW_2_Pin);
  bg_sw_old[3] = !HAL_GPIO_ReadPin(BG_SW_3_GPIO_Port, BG_SW_3_Pin);
  bg_sw_old[4] = !HAL_GPIO_ReadPin(BG_SW_4_GPIO_Port, BG_SW_4_Pin);
  bg_sw_old[5] = !HAL_GPIO_ReadPin(BG_SW_5_GPIO_Port, BG_SW_5_Pin);
  bg_sw_old[6] = !HAL_GPIO_ReadPin(BG_SW_6_GPIO_Port, BG_SW_6_Pin);
  bg_sw_old[7] = !HAL_GPIO_ReadPin(BG_SW_7_GPIO_Port, BG_SW_7_Pin);

  // Setup Color Sensor
  APDS9960 apds;
  apds.begin();
  //enable color sensing mode
  apds.enableColor(true);
  //create some variables to store the color data in
  uint16_t r; 
  uint16_t g; 
  uint16_t b; 
  uint16_t c;
  uint16_t g_cal;
  uint16_t b_cal;
  uint32_t total_cal;
  HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
  HAL_Delay(3000);
  apds.calibrate_sensor(&g_cal, &b_cal, &total_cal);
  
  // Setup find color application
  uint16_t color_find_timer_max = 10000;
  uint16_t color_find_timer = 20;
  bool color_found = true;
  uint8_t color_to_find;
  uint8_t color_to_find_msg;
  uint8_t color_det_timer = 0;

  // Wake switch
  GPIO_PinState wake_sw_state;
  uint8_t wake_det;
  uint8_t inactivity_det = 0;
  uint8_t activity_det;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    ///////////////////////////////////////////////////////////////////////////////////
    // Check if audio is currently being played
    // Check if inactivity detected
    ///////////////////////////////////////////////////////////////////////////////////
    HAL_UART_Receive_IT(&huart1, &uart_data, 1);
    if (uart_ready == 1) {
      if (uart_data < 2) {
        audio_playing = uart_data;
      } else {
        inactivity_det = uart_data - 2;
      }
      uart_ready = 0;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Wake/Sleep Function
    ///////////////////////////////////////////////////////////////////////////////////
    wake_sw_state = HAL_GPIO_ReadPin(WKUP_GPIO_Port, WKUP_Pin);
    if ((wake_sw_state == GPIO_PIN_SET) | inactivity_det) {
      ring_set_all_pixels(ring, rgb_off);
      HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
      bg.clear_display();
      color_find_timer = 50;
      HAL_Delay(100);
    }
    wake_det = 0;
    while (wake_sw_state == GPIO_PIN_SET) {
      // do nothing
      wake_sw_state = HAL_GPIO_ReadPin(WKUP_GPIO_Port, WKUP_Pin);
      wake_det = 1;
    }
    if (wake_det == 1) {
      audio_playing = 1;  // engine start sound will play
      HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
      HAL_Delay(3000);
      apds.calibrate_sensor(&g_cal, &b_cal, &total_cal);
    }

    activity_det = 0;
    while (inactivity_det == 1) {
      sw_states_new = read_sw_states();
      for (uint8_t sw_i = 0; sw_i < 4; sw_i++) {
        if (sw_states_new[sw_i] != sw_states_old[sw_i]) {
          inactivity_det = 0;
        }
      }

      bg_sw_new = read_bg_states();
      for (uint8_t i = 0; i < 8; i++) {
        if (bg_sw_new[i] != bg_sw_old[i]) {
          inactivity_det = 0;
        }
      }

      HAL_UART_Receive_IT(&huart1, &uart_data, 1);
      if (uart_ready == 1) {
        if (uart_data == 2) {
          inactivity_det = 0;
        }
      }
      activity_det = 1;
    }

    if (activity_det == 1) {
      HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
      HAL_Delay(3000);
      apds.calibrate_sensor(&g_cal, &b_cal, &total_cal);
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Toggle switches
    ///////////////////////////////////////////////////////////////////////////////////
    sw_states_new = read_sw_states();
    HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, sw_states_new[0]);
    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, sw_states_new[1]);
    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, sw_states_new[2]);
    for (uint8_t sw_i = 0; sw_i < 4; sw_i++) {
      if ((sw_states_new[sw_i] == GPIO_PIN_SET) && (sw_states_old[sw_i] == GPIO_PIN_RESET)) {
        send_audio(sw_i);
        if (sw_i == 3) {
          ring_dir = !ring_dir;
        }
        break;
      }
    }
    
    for (uint8_t sw_i = 0; sw_i < 4; sw_i++) {
      sw_states_old[sw_i] = sw_states_new[sw_i];
    } 

    ///////////////////////////////////////////////////////////////////////////////////
    // Bar Graph
    ///////////////////////////////////////////////////////////////////////////////////
    bg_sw_new = read_bg_states();
    for (uint8_t i = 0; i < 8; i++) {
      if (bg_sw_new[i] != bg_sw_old[i]) {
        HAL_Delay(100); // allow some debounce
        break;
      }
    }
    bg_sw_new = read_bg_states();
    bg.update(&bg_sw_new[0]);
    for (uint8_t i = 0; i < 8; i++) {
      if (bg_sw_new[i] != bg_sw_old[i]) {
        if (bg_sw_new[i] > bg_sw_old[i]) {
          send_audio(AUDIO_TRIG_BG_UP);
        } else {
          send_audio(AUDIO_TRIG_BG_DOWN);
        }
        bg_sw_old[i] = bg_sw_new[i];
      } 
    }
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Pick color to find
    ///////////////////////////////////////////////////////////////////////////////////
    if (color_find_timer == 0) {
      if (color_found) {
        color_to_find = (HAL_RNG_GetRandomNumber(&hrng) % 7);
        color_to_find_msg = color_to_find + 7;
        if (color_to_find == RED) {
          rgb_new.r = 180;
          rgb_new.g = 0;
          rgb_new.b = 0;
        } else if (color_to_find == ORANGE) {
          rgb_new.r = 180;
          rgb_new.g = 100;
          rgb_new.b = 0;
        } else if (color_to_find == BLUE) {
          rgb_new.r = 0;
          rgb_new.g = 0;
          rgb_new.b = 180;
        } else if (color_to_find == GREEN) {
          rgb_new.r = 0;
          rgb_new.g = 180;
          rgb_new.b = 0;
        } else if (color_to_find == PINK) {
          rgb_new.r = 180;
          rgb_new.g = 10;
          rgb_new.b = 80;
        } else if (color_to_find == PURPLE) {
          rgb_new.r = 120;
          rgb_new.g = 10;
          rgb_new.b = 180;
        } else if (color_to_find == YELLOW) {
          rgb_new.r = 180;
          rgb_new.g = 140;
          rgb_new.b = 0;
        }
        color_found = false;
      }
      send_audio(color_to_find_msg);
      color_find_timer = color_find_timer_max;
    } else {
      color_find_timer--;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Get color sense data and compare to color asked for
    ///////////////////////////////////////////////////////////////////////////////////
    apds.getColorData(&r, &g, &b, &c);
    uint32_t color_total = r + g + b + c;
    if (color_det_timer == 0) {
      if (color_found == false) {
        if (color_total > (total_cal + 5000)) {
          g = g - g_cal; //Adjust for offset from blue PCB 
          b = b - b_cal; //Adjust for offset from blue PCB
          color_t color;
          color = apds.colorSort(r, g, b, color_total, color_to_find);

          if (color != UNKNOWN) {
            if (color == color_to_find) {
              uint8_t color_found_success_msg = color_to_find_msg + 7;
              send_audio(color_found_success_msg);
              ring.setBrightness(100);
              ring_set_all_pixels(ring, rgb_new);
              ring.show();
              ring.setBrightness(200);
              color_found = true;
              color_find_timer = 50;
              HAL_Delay(10000);
            } else {
              uint8_t color_found_fail_msg = color_to_find_msg + 14;
              send_audio(color_found_fail_msg);
              color_det_timer = 25;
            }
          }
        }
      }
    } else {
      color_det_timer--;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // LED ring update
    ///////////////////////////////////////////////////////////////////////////////////
    if (ring_dir == 0) {
      ring.incrRing(rgb_new);
    } else {
      ring.decrRing(rgb_new);
    }

    HAL_Delay(15);
    
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  
    /**Configure the main internal regulator output voltage 
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
	  Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
	  Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
GPIO_PinState * read_sw_states(void) {
  static GPIO_PinState sw_states[4];
  sw_states[0] = HAL_GPIO_ReadPin(SW_0_GPIO_Port, SW_0_Pin);
  sw_states[1] = HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
  sw_states[2] = HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);
  sw_states[3] = HAL_GPIO_ReadPin(SW_3_GPIO_Port, SW_3_Pin);
  return sw_states;
}

uint8_t * read_bg_states(void) {
  static uint8_t bg_sw_states[8];
  bg_sw_states[0] = !HAL_GPIO_ReadPin(BG_SW_0_GPIO_Port, BG_SW_0_Pin);
  bg_sw_states[1] = !HAL_GPIO_ReadPin(BG_SW_1_GPIO_Port, BG_SW_1_Pin);
  bg_sw_states[2] = !HAL_GPIO_ReadPin(BG_SW_2_GPIO_Port, BG_SW_2_Pin);
  bg_sw_states[3] = !HAL_GPIO_ReadPin(BG_SW_3_GPIO_Port, BG_SW_3_Pin);
  bg_sw_states[4] = !HAL_GPIO_ReadPin(BG_SW_4_GPIO_Port, BG_SW_4_Pin);
  bg_sw_states[5] = !HAL_GPIO_ReadPin(BG_SW_5_GPIO_Port, BG_SW_5_Pin);
  bg_sw_states[6] = !HAL_GPIO_ReadPin(BG_SW_6_GPIO_Port, BG_SW_6_Pin);
  bg_sw_states[7] = !HAL_GPIO_ReadPin(BG_SW_7_GPIO_Port, BG_SW_7_Pin);
  return bg_sw_states;
}

void send_audio(uint8_t audio_trig_index) {
  if (audio_playing == 0) {
    HAL_UART_Transmit(&huart1, &audio_trig_index, 1, HAL_MAX_DELAY);
    audio_playing = 1;
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  uart_ready = 1;
}

void Error_Handler(void){
}

/**
  * @brief  This function is executed in case of error occurrence
  * @param  file: The file name as string
  * @param  line: The line in file as a number
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence
  * @param  file: The file name as string
  * @param  line: The line in file as a number
  * @retval None
  */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
