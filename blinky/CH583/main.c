#include <CH58x_common.h>

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
 * eval "$(/home/tools/bin/configure-sdk CH583)"
 * 
 * IMPORTANT
 * 
 * For the "make upload" command to succeed, you need to enable 
 * the chip's simulation interface with WCHISPTool first. You 
 * should then disable it for normal device usage.
 * 
 * CONFIGURATION
 * 
 * On WCH's CH582M-EVT board, user LED to be connected either to PB18
 * or to PB19 by placing a jumper on pins 1 and 2 of pin headers P7
 * or P9 respectively. LED are lit when their GPIO pin is low.
 * 
 * On the YLCH58X board, the LED is connected to PB0 and is lit when
 * its GPIO pin is high.
 * 
 * On the WeAct Studio CH582F board, a blue LED is 
 * connected to PA8 and is lit when its GPIO pin is low.
 * 
 * Change LED_GPIO_PIN below to match your situation.
 */

#define LED_GPIO_PORT 'A'
#define LED_GPIO_PIN GPIO_Pin_8

void gpioModeCfg(char port, uint32_t pin, GPIOModeTypeDef mode) {
	if (port == 'A') {
		GPIOA_ModeCfg(pin, mode);
	} else {
		GPIOB_ModeCfg(pin, mode);
	}
}

void gpioResetBits(char port, uint32_t pin) {
	if (port == 'A') {
		GPIOA_ResetBits(pin);
	} else {
		GPIOB_ResetBits(pin);
	}
}

void gpioInverseBits(char port, uint32_t pin) {
	if (port == 'A') {
		GPIOA_InverseBits(pin);
	} else {
		GPIOB_InverseBits(pin);
	}
}

int main() {
    SetSysClock(CLK_SOURCE_PLL_80MHz);
    gpioResetBits(LED_GPIO_PORT, LED_GPIO_PIN);
    gpioModeCfg(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_ModeOut_PP_5mA);

    while (1) {
    	DelayMs(500);
    	gpioInverseBits(LED_GPIO_PORT, LED_GPIO_PIN);
    }
}
