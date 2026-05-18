/*
 * audio.h
 *
 *  Created on: Mar 16, 2026
 *      Author: MATI
 */

#ifndef INC_AUDIO_H_
#define INC_AUDIO_H_

#include "stm32l4xx_hal.h"

void Audio_Init(void);
void Audio_PlayNote(uint8_t key);
void Audio_Stop(void);

#endif /* INC_AUDIO_H_ */
