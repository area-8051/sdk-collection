#include <gd32vf103.h>

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
 * eval "$(/home/tools/bin/configure-sdk GD32VF103)"
 * 
 * CONFIGURATION
 * 
 * The Longan Nano has 3 LED: red on PC13, green on PA1, blue on PA2.
 * The LED are lit when the GPIO line is low.
 */

void delay1ms(uint32_t count) {
	uint64_t start_mtime, delta_mtime;

	// get current timer value
	uint64_t tmp = get_timer_value();
	
	do {
		start_mtime = get_timer_value();
	} while (start_mtime == tmp);

	// continue counting until the delay time is reached
	do {
		delta_mtime = get_timer_value() - start_mtime;
	} while (delta_mtime < (SystemCoreClock / 4000 * count));
}

#define DELAY 500

int main() {
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2 | GPIO_PIN_1);
    
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    
    while (1) {
    	gpio_bit_reset(GPIOC, GPIO_PIN_13);
    	delay1ms(DELAY);
    	gpio_bit_set(GPIOC, GPIO_PIN_13);
    	gpio_bit_reset(GPIOA, GPIO_PIN_1);
    	delay1ms(DELAY);
    	gpio_bit_set(GPIOA, GPIO_PIN_1);
    	gpio_bit_reset(GPIOA, GPIO_PIN_2);
    	delay1ms(DELAY);
    	gpio_bit_set(GPIOA, GPIO_PIN_2);
    }
}
