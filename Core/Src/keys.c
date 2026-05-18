/*
 * keys.c
 *
 *  Created on: Mar 16, 2026
 *      Author: MATI
 */
#include "keys.h"

// volatile because their values can change in an ISR
volatile uint8_t key_flag = 0;
volatile uint8_t active_key = 255; // 255 is the default state for "no key pressed"
volatile uint32_t last_press_time = 0;


void Keys_Init(void){

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0); // "0" is the highest priority
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

// -- State Machine getters and setters

uint8_t Keys_EventPending(void){
	return key_flag;
}

uint8_t Keys_GetPressed(void){
	return active_key;
}

void Keys_ClearEvent(void){
	key_flag = 0;
}

void EXTI0_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); }
void EXTI1_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1); }
void EXTI2_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2); }
void EXTI3_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3); }
void EXTI4_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4); }

void EXTI9_5_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	uint32_t current_time = HAL_GetTick();

	// If less than 50ms have passed since the last valid press, it's a bounce.
	if((current_time - last_press_time) < 50){
		return;
	}

	last_press_time = current_time;

	if(GPIO_Pin == GPIO_PIN_0){
		active_key = 0;
	}else if(GPIO_Pin == GPIO_PIN_1){
		active_key = 1;
	}else if(GPIO_Pin == GPIO_PIN_2){
		active_key = 2;
	}else if(GPIO_Pin == GPIO_PIN_3){
		active_key = 3;
	}else if(GPIO_Pin == GPIO_PIN_4){
		active_key = 4;
	}else if(GPIO_Pin == GPIO_PIN_5){
		active_key = 5;
	}else if(GPIO_Pin == GPIO_PIN_6){
		active_key = 6;
	}else if(GPIO_Pin == GPIO_PIN_7){
		active_key = 7;
	}

	// Read the physical pin to see if it's currently pressed or released
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_Pin) == GPIO_PIN_RESET) {
		key_flag = 1; // 1 = Note ON (Pressed)
	} else {
		key_flag = 2; // 2 = Note OFF (Released)
	}
}



