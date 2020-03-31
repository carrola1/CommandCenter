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
#include "string.h"
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
#include "motor.h"

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
uint8_t uart_ready = 0;
uint16_t timer_1sec = 0;
uint8_t timer_1sec_flag = 0;
FATFS FatFs;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void wav_file_lut(uint8_t audio_code, char * wav_file);
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
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
  f_mount(&FatFs, "", 1);

  // Audio
  uint8_t new_audio_flag = 0;
  uint8_t audio_is_playing = 0;
  uint8_t audio_code;

  char audio_files[31][32];
  strcpy(audio_files[0], "blip.wav");
  strcpy(audio_files[1], "bloop.wav");
  strcpy(audio_files[2], "blurp.wav");
  strcpy(audio_files[3], "boing.wav");
  strcpy(audio_files[4], "decrement.wav");
  strcpy(audio_files[5], "increment.wav");
  strcpy(audio_files[6], "start_engine.wav");
  strcpy(audio_files[7], "find_red.wav");
  strcpy(audio_files[8], "find_green.wav");
  strcpy(audio_files[9], "find_blue.wav");
  strcpy(audio_files[10], "find_yellow.wav");
  strcpy(audio_files[11], "find_purple.wav");
  strcpy(audio_files[12], "find_pink.wav");
  strcpy(audio_files[13], "find_orange.wav");
  strcpy(audio_files[14], "red.wav");
  strcpy(audio_files[15], "green.wav");
  strcpy(audio_files[16], "blue.wav");
  strcpy(audio_files[17], "yellow.wav");
  strcpy(audio_files[18], "purple.wav");
  strcpy(audio_files[19], "pink.wav");
  strcpy(audio_files[20], "orange.wav");
  strcpy(audio_files[21], "red.wav");
  strcpy(audio_files[22], "green.wav");
  strcpy(audio_files[23], "blue.wav");
  strcpy(audio_files[24], "yellow.wav");
  strcpy(audio_files[25], "purple.wav");
  strcpy(audio_files[26], "pink.wav");
  strcpy(audio_files[27], "orange.wav");
  strcpy(audio_files[28], "pete_the_cat.wav");
  strcpy(audio_files[29], "snuggle_puppy.wav");
  strcpy(audio_files[30], "elmo.wav");

  // Setup Motor PWM
  motor_state motor_position_new;
  motor_state motor_position_last;
  motor_position_last = get_motor_position();
  set_motor_position(motor_position_last);

  // Wake switch
  GPIO_PinState wake_sw_state;

  // States
  typedef enum states {ST_off, ST_sleep, ST_awake} states;
  states state = ST_off;

  // Inactivity detection
  uint16_t inactivity_timeout = 600;
  uint16_t inactivity_timer = inactivity_timeout;

  // Test LED
  HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    wake_sw_state = HAL_GPIO_ReadPin(WKUP_GPIO_Port, WKUP_Pin);
    if (wake_sw_state == GPIO_PIN_SET) {
      state = ST_off;
    }

    switch(state) {
      
      ///////////////////////////////////////////////////////////////////////////////////
      // Off State
      // Key is turned to the off position
      ///////////////////////////////////////////////////////////////////////////////////
      case ST_off:
        if (wake_sw_state == GPIO_PIN_RESET) {
          new_audio_flag = 1;
          audio_code = AUDIO_TRIG_START;
          state = ST_awake;
        }
        HAL_Delay(200);
        break;

      ///////////////////////////////////////////////////////////////////////////////////
      // Sleep State
      // Go here after period of inactivity
      // Look for motor switch change or message from Light controller to wake up
      ///////////////////////////////////////////////////////////////////////////////////
      case ST_sleep:
        HAL_UART_Receive_IT(&huart1, &uart_data, 1);
        motor_position_new = get_motor_position();
        if ((motor_position_new != motor_position_last) || (uart_ready)) {
          state = ST_awake;
          uart_ready = 0;
          inactivity_timer = inactivity_timeout;
          uint8_t inactivity_det_msg = 2;
          HAL_UART_Transmit(&huart1, &inactivity_det_msg, 1, HAL_MAX_DELAY);
        }
        break;
      
      ///////////////////////////////////////////////////////////////////////////////////
      // Wake State
      ///////////////////////////////////////////////////////////////////////////////////
      case ST_awake:

        // Motor control
        motor_position_new = update_motor(motor_position_last);
        if (motor_position_new != motor_position_last) {
          audio_code = 28 + motor_position_new;
          new_audio_flag = 1;
          motor_position_last = motor_position_new;
        }

        // Check for pending audio from light controller
        HAL_UART_Receive_IT(&huart1, &uart_data, 1);
        if (uart_ready == 1) {
          audio_code = uart_data;
          new_audio_flag = 1;
          uart_ready = 0;
        }

        // If new audio comes in while something is already playing
        //  need to close the file and open the a new one.
        if (new_audio_flag == 1) {
          if (audio_is_playing) {
            close_wav();
          }
          open_wav(audio_files[audio_code]);
        }

        // Play a chunk of audio
        if ((new_audio_flag) || (audio_is_playing)) {
          audio_is_playing = 1;
          new_audio_flag = 0;
          uint8_t file_complete = play_chunk();
          inactivity_timer = inactivity_timeout;
          if (file_complete == 1) {
            close_wav();
            audio_is_playing = 0;
          }
        }

        // Detect inactivity / wake
        if (timer_1sec_flag) {
          if (inactivity_timer == 0) {
            uint8_t inactivity_det_msg = 3;
            HAL_UART_Transmit(&huart1, &inactivity_det_msg, 1, HAL_MAX_DELAY);
            state = ST_sleep;
          } else {
            inactivity_timer--;
          }
          timer_1sec_flag = 0;
        }
        break;

      default:
        state = ST_awake;

    }

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
void HAL_SYSTICK_Callback(void) {
  if (timer_1sec == 1000) {
    timer_1sec = 0;
    timer_1sec_flag = 1;
  } else {
    timer_1sec++;
  }
  return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  uart_ready = 1;
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