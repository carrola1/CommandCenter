/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "dma.h"
#include "diskio.h"
#include "ff.h"
#include "lptim.h"
#include "i2s.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "wav_player.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t uart_data;
uint8_t uart_ready = 1;
uint8_t audio_playing = 0;
uint8_t audio_drop = 0;
FATFS FatFs;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2S2_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_LPTIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(100);
  HAL_GPIO_WritePin(PWR_ENB_GPIO_Port, PWR_ENB_Pin, GPIO_PIN_SET);
  
  // Mount SD Card
  FRESULT fr;     /* FatFs return code */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
  fr = f_mount(&FatFs, "", 1);
  
  // Setup Motor PWM
  uint8_t motor_state_0;
  uint8_t motor_state_1;
  uint8_t motor_state_2;
  uint8_t motor_state_last;
  motor_state_0 = HAL_GPIO_ReadPin(MOTOR_SW_0_GPIO_Port, MOTOR_SW_0_Pin);
  motor_state_1 = HAL_GPIO_ReadPin(MOTOR_SW_1_GPIO_Port, MOTOR_SW_1_Pin);
  motor_state_2 = HAL_GPIO_ReadPin(MOTOR_SW_2_GPIO_Port, MOTOR_SW_2_Pin);
  if (motor_state_0 == 0) {
    motor_state_last = 0;
    HAL_LPTIM_PWM_Start(&hlptim1, 0x1388, 0x0258);
  } else if (motor_state_1 == 0) {
    motor_state_last = 1;
    HAL_LPTIM_PWM_Start(&hlptim1, 0x1388, 0x0190);
  } else {
    motor_state_last = 2;
    HAL_LPTIM_PWM_Start(&hlptim1, 0x1388, 0x00AF);
  }
  
  // Wake switch
  GPIO_PinState wake_sw_state;

  // Test LED
  HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    wake_sw_state = HAL_GPIO_ReadPin(WKUP_GPIO_Port, WKUP_Pin);
    while (wake_sw_state == GPIO_PIN_SET) {
      // do nothing
      wake_sw_state = HAL_GPIO_ReadPin(WKUP_GPIO_Port, WKUP_Pin);
    }
    
    // Motor control
    motor_state_0 = HAL_GPIO_ReadPin(MOTOR_SW_0_GPIO_Port, MOTOR_SW_0_Pin);
    motor_state_1 = HAL_GPIO_ReadPin(MOTOR_SW_1_GPIO_Port, MOTOR_SW_1_Pin);
    motor_state_2 = HAL_GPIO_ReadPin(MOTOR_SW_2_GPIO_Port, MOTOR_SW_2_Pin);
    if ((motor_state_0 == 0) & (motor_state_last != 0)) {
      __HAL_LPTIM_COMPARE_SET(&hlptim1, 0x0258);
      HAL_Delay(100);
      play_wav("pete_the_cat.wav");
      HAL_UART_Receive_IT(&huart1, &uart_data, 1);	// clear pending interrupt
      uart_ready = 0;
      motor_state_last = 0;
    } else if ((motor_state_1 == 0) & (motor_state_last != 1)) {
      __HAL_LPTIM_COMPARE_SET(&hlptim1, 0x0190);
      HAL_Delay(100);
      play_wav("blues_clues.wav");
      HAL_UART_Receive_IT(&huart1, &uart_data, 1);	// clear pending interrupt
      uart_ready = 0;
      motor_state_last = 1;
    } else if ((motor_state_2 == 0) & (motor_state_last != 2)) {
      __HAL_LPTIM_COMPARE_SET(&hlptim1, 0x00AF);
      HAL_Delay(100);
      play_wav("elmo.wav");
      HAL_UART_Receive_IT(&huart1, &uart_data, 1);	// clear pending interrupt
      uart_ready = 0;
      motor_state_last = 2;
    }

    HAL_UART_Receive_IT(&huart1, &uart_data, 1);
    if (uart_ready == 1) {
      if (uart_data == 0) {
        play_wav("blip.wav");
      } else if (uart_data == 1) {
        play_wav("bloop.wav");
      } else if (uart_data == 2) {
        play_wav("blurp.wav");
      } else if (uart_data == 3){
        play_wav("boing.wav");
      } else if (uart_data == 4){
        play_wav("ratchet.wav");
      } else if (uart_data == 5) {
    	  play_wav("find_red.wav");
      } else if (uart_data == 6) {
      	play_wav("find_green.wav");
      } else if (uart_data == 7) {
      	play_wav("find_blue.wav");
      } else if (uart_data == 8) {
      	play_wav("find_yellow.wav");
      } else if (uart_data == 9) {
      	play_wav("find_purple.wav");
      } else if (uart_data == 10) {
      	play_wav("find_pink.wav");
      } else if (uart_data == 11) {
      	play_wav("find_orange.wav");
      } else if (uart_data == 12) {
        play_wav("awesome.wav");
        HAL_Delay(100);
		    play_wav("red.wav");
      } else if (uart_data == 13) {
        play_wav("awesome.wav");
        HAL_Delay(100);
        play_wav("green.wav");
      } else if (uart_data == 14) {
        play_wav("awesome.wav");
        HAL_Delay(100);
        play_wav("blue.wav");
      } else if (uart_data == 15) {
        play_wav("you_did_it.wav");
        HAL_Delay(100);
        play_wav("yellow.wav");
      } else if (uart_data == 16) {
        play_wav("good_job.wav");
        HAL_Delay(100);
        play_wav("purple.wav");
      } else if (uart_data == 17) {
        play_wav("good_job.wav");
        HAL_Delay(100);
        play_wav("pink.wav");
      } else if (uart_data == 18) {
        play_wav("you_did_it.wav");
        HAL_Delay(100);
        play_wav("orange.wav");
      } else {
    	  play_wav("try_again.wav");
      }
      if (uart_data < 19) {
        // Clear pending interrupt except for after try_again
    	// There is often a success immediately following a try_again
    	HAL_UART_Receive_IT(&huart1, &uart_data, 1);
        uart_ready = 0;
      }
    }

    HAL_Delay(10);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
    _Error_Handler(__FILE__, __LINE__);
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
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_LPTIM1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.LptimClockSelection = RCC_LPTIM1CLKSOURCE_PCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
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
  audio_drop = audio_playing;
}
/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
