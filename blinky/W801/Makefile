# SPDX-License-Identifier: BSD-2-Clause
# 
# Copyright (c) 2022 Vincent DEFERT. All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions 
# are met:
# 
# 1. Redistributions of source code must retain the above copyright 
# notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright 
# notice, this list of conditions and the following disclaimer in the 
# documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
# POSSIBILITY OF SUCH DAMAGE.

# Usage ----------------------------------------------------------------
#
# IMPORTANT!
#
#   The following command MUST be executed before running make:
#   eval "$(/home/tools/bin/configure-sdk W801)"
#
# Build executable in release mode:
#   make
#
# Build executable in debug mode:
#   make BUILD_MODE=debug
#
# Build documentation (generated under ./build/):
#   make doc
#
# Upload .img firmware to MCU:
#   make upload-image
#
#   IMPORTANT: wm_tool will wait for you to push the RESET button 
#   before starting the firmware upload.
#
# Upload .fls firmware to MCU:
#   make upload-flash
#
#   Same as upload-image, but sends a reset command to the board
#   when upload is finished, so you don't have to push the RESET
#   button to start the new firmware.
#
# Upload .fls firmware to MCU and capture the log output by the device:
#   make upload+run
#
# Erase MCU flash:
#   make erase
#
# Force rebuilding dependencies:
#   make depend
#
# Open serial console in new window:
#   make console
#
# Clean project (remove all build files):
#   make clean

# Project-specific settings --------------------------------------------

PROJECT_NAME := blinky

CONSOLE_BAUDRATE := 115200
CONSOLE_PORT := ttyUSB1

ISP_PORT := ttyUSB0
ISP_BAUDRATE := 115200

SRCS := \
	main.c

# Toolchain-specific settings ------------------------------------------

SHARED_FLAGS := -mcpu=ck804ef -mhard-float

CC := ${TOOLCHAIN_PREFIX}-gcc
CFLAGS := $(SHARED_FLAGS) -DTLS_CONFIG_CPU_XT804=1 -DGCC_COMPILE=1 \
	${SDK_INCLUDES} -Wall -fdata-sections -ffunction-sections

LD := ${TOOLCHAIN_PREFIX}-gcc
# Using: -Wl,--start-group ${SDK_LIBS} -Wl,--end-group
# would be much preferable to: -Wl,--whole-archive ${SDK_LIBS} -Wl,--no-whole-archive
# but for some reason, it doesn't work with this toolchain.
LDFLAGS := $(SHARED_FLAGS) -nostartfiles -lm \
	-Wl,--gc-sections -Wl,-zmax-page-size=1024 -L${SDK_LIBDIR} \
	-Wl,--whole-archive ${SDK_LIBS} -Wl,--no-whole-archive \
	-Wl,-T${SDK_LDSCRIPT}

OBJCOPY := ${TOOLCHAIN_PREFIX}-objcopy

OBJDUMP = ${TOOLCHAIN_PREFIX}-objdump

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -Og -g -DWM_SWD_ENABLE=1
	BUILD_DIR := debug
else
	CFLAGS += -Os
	BUILD_DIR := release
endif

IMG_TYPE := 1
IMG_HEADER := 8010000
RUN_ADDRESS := 8010400
UPD_ADDRESS := 8010000

PRIKEY_SEL := 0
SIGNATURE := 0
CODE_ENCRYPT := 0
SIGN_PUBKEY_SRC := 0

SECBOOT_HEADER_POS  := 8002000
SECBOOT_ADDRESS_POS := 8002400
SEC_BOOT     := ${SDK_DIR}/tools/w800_secboot
SEC_BOOT_BIN := ${SDK_DIR}/tools/w800_secboot.bin
SEC_BOOT_IMG := ${SDK_DIR}/tools/w800_secboot.img

CA_PATH := ${SDK_DIR}/tools/ca

BUILD_ROOT := build

OBJDIR := $(BUILD_ROOT)/$(BUILD_DIR)
ARTIFACT_PREFIX := $(OBJDIR)/$(PROJECT_NAME)
DEP_FILE := $(OBJDIR)/dependencies
LDFLAGS += -Wl,-ckmap=$(ARTIFACT_PREFIX).map

ifeq ($(SIGNATURE),1)
	FW_FILE := $(ARTIFACT_PREFIX)_sign_ota.img
else
	FW_FILE := $(ARTIFACT_PREFIX)_ota.img
endif

DEP_FILE := $(OBJDIR)/dependencies
ELF_FILE = $(ARTIFACT_PREFIX).elf
LST_FILE = $(ARTIFACT_PREFIX).lst
MAP_FILE = $(ARTIFACT_PREFIX).map
BIN_FILE = $(ARTIFACT_PREFIX).bin

OBJS := $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

# Rules ----------------------------------------------------------------

.PHONY: all clean doc upload-image upload-flash upload+run erase console checkenv

all: checkenv $(DEP_FILE) $(FW_FILE) $(LST_FILE)


checkenv:
	@/home/tools/bin/check-sdk-env W801

clean:
	rm -rf $(BUILD_ROOT)

doc:
	doxygen ${TOOLCHAIN_ETC}/doxygen.conf

upload-image:
	${SDK_FLASH_TOOL} -c $(ISP_PORT) -rs at -ds $(ISP_BAUDRATE) -dl $(ARTIFACT_PREFIX).img

upload:
	${SDK_FLASH_TOOL} -c $(ISP_PORT) -rs at -ds $(ISP_BAUDRATE) -dl $(ARTIFACT_PREFIX).fls

upload+run:
	${SDK_FLASH_TOOL} -c $(ISP_PORT) -rs at -ds $(ISP_BAUDRATE) -dl $(ARTIFACT_PREFIX).fls -sl str -ws 115200

erase:
	${SDK_FLASH_TOOL} -c $(ISP_PORT) -rs at -eo all

console:
	mate-terminal -e "minicom -b $(CONSOLE_BAUDRATE) -D /dev/$(CONSOLE_PORT)"

-include $(DEP_FILE)

$(DEP_FILE): $(SRCS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -MT $(patsubst %.c, $(OBJDIR)/%.o, $<) -MF /dev/stdout -o /dev/null -c $< >> $(DEP_FILE)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(FW_FILE): $(OBJS)
	$(LD) $(LDFLAGS) -o $(ELF_FILE) $^
	$(OBJCOPY) -O binary $(ELF_FILE) $(BIN_FILE)

ifeq ($(CODE_ENCRYPT),1)
	openssl enc -aes-128-ecb -in $(BIN_FILE) -out $(ARTIFACT_PREFIX)_enc.bin -K 30313233343536373839616263646566 -iv 01010101010101010101010101010101
	openssl rsautl -encrypt -in $(CA_PATH)/key.txt -inkey $(CA_PATH)/capub_$(PRIKEY_SEL).pem -pubin -out $(OBJDIR)/key_en.dat
	cat $(ARTIFACT_PREFIX)_enc.bin $(OBJDIR)/key_en.dat > $(ARTIFACT_PREFIX)_enc_key.bin
	cat $(ARTIFACT_PREFIX)_enc_key.bin $(CA_PATH)/capub_$(PRIKEY_SEL)_N.dat > $(ARTIFACT_PREFIX)_enc_key_N.bin
	${SDK_FLASH_TOOL} -b $(ARTIFACT_PREFIX)_enc_key_N.bin -it $(IMG_TYPE) -fc 0 -ra $(RUN_ADDRESS) -ih $(IMG_HEADER) -ua $(UPD_ADDRESS) -nh 0 -un 0 -o $(ARTIFACT_PREFIX)
else
	${SDK_FLASH_TOOL} -b $(BIN_FILE) -fc 0 -it $(IMG_TYPE) -ih $(IMG_HEADER) -ra $(RUN_ADDRESS) -ua $(UPD_ADDRESS) -nh 0 -un 0 -o $(ARTIFACT_PREFIX)
endif

	${SDK_FLASH_TOOL} -b  $(SEC_BOOT_BIN) -fc 0 -it 0 -ih $(SECBOOT_HEADER_POS) -ra $(SECBOOT_ADDRESS_POS) -ua $(UPD_ADDRESS) -nh $(IMG_HEADER) -un 0 -o $(SEC_BOOT)

ifeq ($(SIGNATURE),1)
	openssl dgst -sign $(CA_PATH)/cakey.pem -sha1 -out $(ARTIFACT_PREFIX)_sign.dat $(ARTIFACT_PREFIX).img
	cat $(ARTIFACT_PREFIX).img $(ARTIFACT_PREFIX)_sign.dat > $(ARTIFACT_PREFIX)_sign.img
	cat $(SEC_BOOT_IMG) $(ARTIFACT_PREFIX)_sign.img > $(ARTIFACT_PREFIX).fls
	${SDK_FLASH_TOOL} -b $(ARTIFACT_PREFIX)_sign.img -fc 1 -it $(IMG_TYPE) -ih $(IMG_HEADER) -ra $(RUN_ADDRESS) -ua $(UPD_ADDRESS) -nh 0 -un 0 -o $(ARTIFACT_PREFIX)_sign
	mv $(ARTIFACT_PREFIX)_sign_gz.img $(FW_FILE)
else
	cat $(SEC_BOOT_IMG) $(ARTIFACT_PREFIX).img > $(ARTIFACT_PREFIX).fls
	${SDK_FLASH_TOOL} -b $(ARTIFACT_PREFIX).img -fc 1 -it $(IMG_TYPE) -ih $(IMG_HEADER) -ra $(RUN_ADDRESS) -ua $(UPD_ADDRESS) -nh 0 -un 0 -o $(ARTIFACT_PREFIX)
	mv $(ARTIFACT_PREFIX)_gz.img $(FW_FILE)
endif

$(LST_FILE): $(ELF_FILE)
	$(OBJDUMP) --source --all-headers --demangle --line-numbers --wide $(ELF_FILE) > $(LST_FILE)
