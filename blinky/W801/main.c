#include <wm_include.h>
#include <os/wm_osal.h>

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
 * eval "$(/home/tools/bin/configure-sdk W801)"
 */

static enum tls_io_name leds[] = {
	WM_IO_PB_05,
	WM_IO_PB_25,
	WM_IO_PB_26,
	WM_IO_PB_18,
	WM_IO_PB_17,
	WM_IO_PB_16,
	WM_IO_PB_11,
};

#define LED_COUNT (sizeof(leds) / sizeof(enum tls_io_name))

// Task stack MUST be static according to ${SDK_DIR}/include/os/wm_osal.h.
// OS_STK is unsigned int, so array size is a number of words.
static OS_STK blinkyTaskStack[128]; 

static void blinkyTask(void *param) {
	for (int i = 0; i < LED_COUNT; i++) {
		tls_gpio_cfg(leds[i], WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	}
	
	while (1) {
		for (int i = 0; i < LED_COUNT; i++) {
			tls_gpio_write(leds[i], tls_gpio_read(leds[i]));
			tls_os_time_delay(250);
			tls_gpio_write(leds[i], tls_gpio_read(leds[i]));
		}
	}
}

// See ${SDK_DIR}/include/wm_include.h
void UserMain() {
	// See ${SDK_DIR}/include/os/wm_osal.h
	tls_os_task_create(
		NULL, 
		"Blinky task",
		blinkyTask,
		NULL, // Task initialisation parameters, i.e. void *param of blinkyTask()
		(u8 *) blinkyTaskStack,
		sizeof(blinkyTaskStack),
		61, // Priority, must be unique (u32)
		0   // Task flags (u32)
	);
}
