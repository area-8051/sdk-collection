#include <ch32v10x.h>
#include <debug.h>

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
 * eval "$(/home/tools/bin/configure-sdk CH32V103)"
 * 
 * IMPORTANT
 * 
 * For the "make upload" command to succeed, you need to enable 
 * the chip's simulation interface with WCHISPTool first. You 
 * should then disable it for normal device usage.
 * 
 * CONFIGURATION
 * 
 * On WCH's CH32V103-EVT board, 2 user LED are available on the P4 pin 
 * header, to be connected to the GPIO pins of your choice.
 * LED are lit when their GPIO pin is low.
 * 
 * On the WeAct Studio CH32V104C8T6 BluePill+ board, a blue LED is 
 * connected to PB2 and is lit when its GPIO pin is high.
 * 
 * Change LED_GPIO_PIN below to match your situation.
 */

#define LED_GPIO_PIN GPIO_Pin_2

int main() {
	Delay_Init();
	
	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	int i = 0;

    while (1) {
    	Delay_Ms(125);
    	GPIO_WriteBit(GPIOB, LED_GPIO_PIN, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}
