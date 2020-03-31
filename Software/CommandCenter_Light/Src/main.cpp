
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
#include "light_control_funcs.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t uart_ready = 1;
uint8_t uart_data;

uint8_t bg_sw_states[8];
GPIO_PinState tog_sw_states[3];
GPIO_PinState push_sw_state;

// Color State Machine
color_states color_state = ST_color_calibrate;

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
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_RNG_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(200);

  // Setup and initialize Dotstars
  DotStar ring = DotStar(20, DOTSTAR_BGR);
  ring.begin(); // Initialize pins for output
  RGB_VALS rgb_off;
  rgb_off.r = 0; rgb_off.g = 0; rgb_off.b = 0;
  ring.setBrightness(200);
  uint8_t ring_dir = 0;

  // Initialize LED driver
  BarGraph bg=BarGraph(8,40);
  bg.begin();

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
  
  // Setup find color application
  uint16_t color_request_timeout = 50;
  uint16_t color_request_timer = color_request_timeout;
  uint16_t color_search_timeout = 10000;
  uint16_t color_search_timer = color_search_timeout;
  uint16_t song_wait_timeout = 700;
  uint16_t song_wait_timer = song_wait_timeout;
  uint16_t request_wait_timeout = 250;
  uint16_t request_wait_timer = request_wait_timeout;
  color_t color;
  uint32_t color_total;
  RGB_VALS rgb_new;
  uint8_t color_to_find;
  uint8_t color_audio_message;

  // Wake switch
  GPIO_PinState wake_sw_state = GPIO_PIN_SET;
  GPIO_PinState wake_sw_state_new;

  // States
  typedef enum pwr_states {ST_pwr_off, ST_pwr_sleep, ST_pwr_awake} pwr_states;
  pwr_states pwr_state = ST_pwr_off;

  uint8_t tog_sw_change; 
  uint8_t bg_sw_change;
  uint8_t push_sw_change;

  turn_all_leds_off(ring, bg);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // Check wake/sleep switch
    wake_sw_state_new = HAL_GPIO_ReadPin(WKUP_GPIO_Port, WKUP_Pin);
    if ((wake_sw_state_new == GPIO_PIN_SET) && (wake_sw_state == GPIO_PIN_RESET)) {
      turn_all_leds_off(ring, bg);
      pwr_state = ST_pwr_off;
    }
    wake_sw_state = wake_sw_state_new;

    switch(pwr_state) {
      
      case ST_pwr_off:
        if (wake_sw_state == GPIO_PIN_RESET) {
          read_tog_sw_states();
          read_bg_sw_states();
          read_push_sw_state();
          bg.update(&bg_sw_states[0]);
          color_state = ST_color_calibrate;
          pwr_state = ST_pwr_awake;
        }
        break;

      case ST_pwr_sleep:
        HAL_UART_Receive_IT(&huart1, &uart_data, 1);
        tog_sw_change = check_tog_sw_states();
        bg_sw_change = check_bg_sw_states();
        push_sw_change = check_push_sw_state();
        if (tog_sw_change || bg_sw_change || push_sw_change || uart_ready) {
          uart_ready = 0;
          uint8_t wakeup_message = 0;
          HAL_UART_Transmit(&huart1, &wakeup_message, 1, HAL_MAX_DELAY);
          pwr_state = ST_pwr_off;
        } else {
          HAL_Delay(100);
        }
        break;

      case ST_pwr_awake:

        ///////////////////////////////////////////////////////////////////////////////////
        // Get new switch states
        ///////////////////////////////////////////////////////////////////////////////////
        update_tog_sw_states();
        update_bg(bg);
        update_push_sw_state(&ring_dir);

        ///////////////////////////////////////////////////////////////////////////////////
        // Color State Machine
        ///////////////////////////////////////////////////////////////////////////////////
        switch(color_state) {
          
          /////////////////////////// Calibrate sensor ////////////////////////////////////
          case ST_color_calibrate:
            HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
            HAL_Delay(3000);
            apds.calibrate_sensor(&g_cal, &b_cal, &total_cal);
            color_state = ST_color_request;
            break;
          
          /////////////////////// Request a new color ////////////////////////////////////
          case ST_color_request:
            if (color_request_timer == 0) {
              color_to_find = (HAL_RNG_GetRandomNumber(&hrng) % 7);
              rgb_new = rgb_lut(color_to_find);
              set_all_pixels(ring, rgb_new);
              color_audio_message = color_to_find + 7;
              send_audio(color_audio_message);
              color_request_timer = color_request_timeout;
              color_state = ST_color_wait_for_request;
            } else {
              color_request_timer--;
            }
            break;

          //////////////////// Read color sensor and process /////////////////////////////
          case ST_color_search:
            
            // Update ring
            if (ring_dir == 0) {
              ring.incrRing(rgb_new);
            } else {
              ring.decrRing(rgb_new);
            }

            // Repeat request after timeout
            if (color_search_timer == 0) {
              color_search_timer = color_search_timeout;
              color_audio_message = color_to_find + 7;
              send_audio(color_audio_message);
            } else {
              color_search_timer--;
            }
            
            apds.getColorData(&r, &g, &b, &c);
            color_total = r + g + b + c;
            if (color_total > (total_cal + 5000)) {
              g = g - g_cal; //Adjust for offset from blue PCB 
              b = b - b_cal; //Adjust for offset from blue PCB
              color = apds.colorSort(r, g, b, color_total, color_to_find);
              if (color != UNKNOWN) {
                if (color == color_to_find) {
                  color_state = ST_color_response_success;
                } else {
                  color_state = ST_color_response_fail;
                }
              }
            }
            break;

          ////////////////////////////// Color found! /////////////////////////////////
          case ST_color_response_success:
            color_audio_message = color_to_find + 14;
            send_audio(color_audio_message);
            set_all_pixels(ring, rgb_new);
            color_state = ST_color_wait_for_song;
            break;

          ////////////////////////// Wrong color found! //////////////////////////////
          case ST_color_response_fail:
            color_audio_message = color + 21;
            send_audio(color_audio_message);
            rgb_new = rgb_lut(color);
            set_all_pixels(ring, rgb_new);
            color_state = ST_color_wait_for_song;
            break;

          ////////////////////////// Wait for request to play ////////////////////////////
          case ST_color_wait_for_request:
            if (request_wait_timer == 0) {
              request_wait_timer = request_wait_timeout;
              color_state = ST_color_search;
            } else {
              request_wait_timer--;
            }
            break;
          
          ////////////////////////// Wait for song to play ////////////////////////////
          case ST_color_wait_for_song:
            if (song_wait_timer == 0) {
              song_wait_timer = song_wait_timeout;
              set_all_pixels(ring, rgb_off);
              color_state = ST_color_request;
            } else {
              song_wait_timer--;
            }
            break;
          
        }

        ///////////////////////////////////////////////////////////////////////////////////
        // Check if inactivity detected
        ///////////////////////////////////////////////////////////////////////////////////
        HAL_UART_Receive_IT(&huart1, &uart_data, 1);
        if (uart_ready == 1) {
          if (uart_data == 3) {
            turn_all_leds_off(ring, bg);
            pwr_state = ST_pwr_sleep;
          }
          uart_ready = 0;
        }
    }
    
    // Slow down loop
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
