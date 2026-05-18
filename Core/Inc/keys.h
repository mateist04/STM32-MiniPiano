/*
 * keys.h
 *
 *  Created on: Mar 16, 2026
 *      Author: MATI
 */

#ifndef INC_KEYS_H_
#define INC_KEYS_H_

#include "stm32l4xx_hal.h"

void Keys_Init(void);
uint8_t Keys_EventPending(void);
uint8_t Keys_GetPressed(void);
void Keys_ClearEvent(void);

#endif /* INC_KEYS_H_ */
