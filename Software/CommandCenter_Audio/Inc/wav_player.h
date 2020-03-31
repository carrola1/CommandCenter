#ifndef WAV_PLAYER_H
#define WAV_PLAYER_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "stm32l0xx_hal.h"
#include "diskio.h"
#include "ff.h"
#include "i2s.h"

//const uint16_t buf_size = 256;

uint8_t wav_buf[512];
UINT bytes_read;

FRESULT fr;     /* FatFs return code */

void play_wav(char wav_file[32]);
void open_wav(char wav_file[32]);
uint8_t play_chunk(void);
void close_wav(void);


#ifdef __cplusplus
}
#endif
#endif
