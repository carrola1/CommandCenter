#include "wav_player.h"

static FIL fil;        // audio file
static uint16_t audio_buf_0[256];
static uint16_t audio_buf_1[256];
static uint16_t *audio_buf_ptr;
static uint16_t *audio_buf_ptr_start;

void play_wav(char wav_file[32]) {
    HAL_GPIO_WritePin(AUDIO_SD_N_0_GPIO_Port, AUDIO_SD_N_0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AUDIO_SD_N_1_GPIO_Port, AUDIO_SD_N_1_Pin, GPIO_PIN_SET);
    fr = f_open(&fil, wav_file, FA_READ);  // open file
    //f_lseek(&fil, 76);                      // move to data region of .wav
    audio_buf_ptr = audio_buf_0;        // point to buffer 0 first
    audio_buf_ptr_start = audio_buf_0;

    while(1) {
        f_read(&fil, &wav_buf[0], 512, &bytes_read);

        //////////// End of File ////////////
        if (bytes_read < 512) {
            f_close(&fil);
            HAL_GPIO_WritePin(AUDIO_SD_N_0_GPIO_Port, AUDIO_SD_N_0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(AUDIO_SD_N_1_GPIO_Port, AUDIO_SD_N_1_Pin, GPIO_PIN_RESET);
            return;
        }
        //////////// End of File ////////////

        // convert raw bytes from wav file into 16-bit audio samples
        for (int ii=0; ii<511; ii+=2) {
            *audio_buf_ptr = ((uint16_t)wav_buf[ii+1] << 8) | (uint16_t)wav_buf[ii];
            if (*audio_buf_ptr > 32767) {
                *audio_buf_ptr = (*audio_buf_ptr >> 1) + 32768;
            } else {
                *audio_buf_ptr = *audio_buf_ptr >> 1;
            }
            audio_buf_ptr++;
        }

        while (hi2s2.State != HAL_I2S_STATE_READY);    // Wait for I2S to be ready
        HAL_I2S_Transmit_DMA(&hi2s2, audio_buf_ptr_start, 256);    // play buffer
        
        // ping pong buffer
        if (audio_buf_ptr_start == &audio_buf_0[0]) {
            audio_buf_ptr = &audio_buf_1[0];
            audio_buf_ptr_start = &audio_buf_1[0];
        } else {
            audio_buf_ptr = &audio_buf_0[0];
            audio_buf_ptr_start = &audio_buf_0[0];
        }
    }
    
}

void open_wav(char wav_file[32]) {
  HAL_GPIO_WritePin(AUDIO_SD_N_0_GPIO_Port, AUDIO_SD_N_0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(AUDIO_SD_N_1_GPIO_Port, AUDIO_SD_N_1_Pin, GPIO_PIN_SET);
  fr = f_open(&fil, wav_file, FA_READ);  // open file
  audio_buf_ptr = audio_buf_0;        // point to buffer 0 first
  audio_buf_ptr_start = audio_buf_0;
}

uint8_t play_chunk(void) {
  f_read(&fil, &wav_buf[0], 512, &bytes_read);

  //////////// End of File ////////////
  if (bytes_read < 512) {
    return 1;
  }

  // convert raw bytes from wav file into 16-bit audio samples
  for (int ii=0; ii<511; ii+=2) {
      *audio_buf_ptr = ((uint16_t)wav_buf[ii+1] << 8) | (uint16_t)wav_buf[ii];
      if (*audio_buf_ptr > 32767) {
          *audio_buf_ptr = (*audio_buf_ptr >> 1) + 32768;
      } else {
          *audio_buf_ptr = *audio_buf_ptr >> 1;
      }
      audio_buf_ptr++;
  }

  while (hi2s2.State != HAL_I2S_STATE_READY);    // Wait for I2S to be ready
  HAL_I2S_Transmit_DMA(&hi2s2, audio_buf_ptr_start, 256);    // play buffer
  
  // ping pong buffer
  if (audio_buf_ptr_start == &audio_buf_0[0]) {
      audio_buf_ptr = &audio_buf_1[0];
      audio_buf_ptr_start = &audio_buf_1[0];
  } else {
      audio_buf_ptr = &audio_buf_0[0];
      audio_buf_ptr_start = &audio_buf_0[0];
  }

  return 0;

}

void close_wav(void) {
  f_close(&fil);
  HAL_GPIO_WritePin(AUDIO_SD_N_0_GPIO_Port, AUDIO_SD_N_0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(AUDIO_SD_N_1_GPIO_Port, AUDIO_SD_N_1_Pin, GPIO_PIN_RESET);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef* hi2s) {
    return;
}
