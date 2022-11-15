#include <driver/wm_hal.h>

/*
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * Copyright (c) 2022 Vincent DEFERT. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the 
 * documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * REMINDER
 * 
 * Don't forget to run the following command before using make:
 * eval "$(/home/tools/bin/configure-sdk W806)"
 */

#define readl(addr) ({unsigned int __v = (*(volatile unsigned int *) (addr)); __v;})

__attribute__((isr)) void CORET_IRQHandler(void) {
    readl(0xE000E010);
    HAL_IncTick();
}

GPIO_InitTypeDef leds[] = {
	{ .Pin = GPIO_PIN_0, .Mode = GPIO_MODE_OUTPUT, .Pull = GPIO_NOPULL },
	{ .Pin = GPIO_PIN_1, .Mode = GPIO_MODE_OUTPUT, .Pull = GPIO_NOPULL },
	{ .Pin = GPIO_PIN_2, .Mode = GPIO_MODE_OUTPUT, .Pull = GPIO_NOPULL },
};

#define LED_COUNT (sizeof(leds) / sizeof(GPIO_InitTypeDef))

int main() {
	SystemClock_Config(CPU_CLK_240M);
	
	for (int i = 0; i < LED_COUNT; i++) {
		HAL_GPIO_Init(GPIOB, leds + i);
		HAL_GPIO_WritePin(GPIOB, leds[i].Pin, GPIO_PIN_SET);
	}
	
	while (1) {
		for (int i = 0; i < LED_COUNT; i++) {
			HAL_GPIO_TogglePin(GPIOB, leds[i].Pin);
			HAL_Delay(500);
			HAL_GPIO_TogglePin(GPIOB, leds[i].Pin);
		}
	}
}
