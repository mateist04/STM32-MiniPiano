/*
 * audio.c
 *
 *  Created on: Mar 16, 2026
 *      Author: MATI
 */

#include "audio.h"

// We create the timer handle here so this module is fully independent
TIM_HandleTypeDef htim2;

// The pre-calculated ARR values for our 80 MHz clock
const uint32_t piano_notes[8] = {
    305774, // 0: C4 (Do)
    272423, // 1: D4 (Re)
    242695, // 2: E4 (Mi)
    229074, // 3: F4 (Fa)
    204081, // 4: G4 (Sol)
    181817, // 5: A4 (La)
    161982, // 6: B4 (Si)
    152890  // 7: C5 (Do)
};

void Audio_Init(void) {
    // 1. Turn on the clocks
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 2. Configure PA5 specifically for Timer 2 (Alternate Function 1)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Configure the Base Timer (No Prescaler)
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0; // Starts at 0, updated when a key is pressed
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim2);

    // 4. Configure Channel 1 for PWM Output
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;   // Starts silent (0% duty cycle)
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

    // 5. Start the timer running in the background
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void Audio_PlayNote(uint8_t key) {
    if (key > 7) return; // Prevent array out-of-bounds crash

    uint32_t arr_val = piano_notes[key];

    // Set the new frequency (Pitch)
    __HAL_TIM_SET_AUTORELOAD(&htim2, arr_val);

    // Set the duty cycle to 50% (Volume/Square Wave Shape)
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, arr_val / 2);
}

void Audio_Stop(void) {
    // Drop the pulse width to 0 to instantly silence the pin
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
}
