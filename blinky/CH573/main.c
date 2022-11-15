#include "CH58x_common.h"

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
 * eval "$(/home/tools/bin/configure-sdk CH573)"
 * 
 * IMPORTANT
 * 
 * For the "make upload" command to succeed, you need to enable 
 * the chip's simulation interface with WCHISPTool first. You 
 * should then disable it for normal device usage.
 * 
 * CONFIGURATION
 * 
 * On WCH's CH573F-EVT board, 2 user LED are exposed on P8.
 * A jumper must be placed on P5, and a jumper wire must 
 * connect the chosen pin of P8 to the PA8 GPIO pin.
 * The LED is lit when its GPIO pin is low.
 */

#define LED_GPIO_PIN GPIO_Pin_8

int main() {
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    GPIOA_SetBits(LED_GPIO_PIN);
    GPIOA_ModeCfg(LED_GPIO_PIN, GPIO_ModeOut_PP_5mA);

    while (1) {
    	DelayMs(125);
    	GPIOA_InverseBits(LED_GPIO_PIN);
    }
}
