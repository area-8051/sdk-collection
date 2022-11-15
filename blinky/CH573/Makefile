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
#   eval "$(/home/tools/bin/configure-sdk CH573)"
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
# Upload firmware to MCU:
#   make upload
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

PROJECT_NAME = blinky

CONSOLE_BAUDRATE = 115200# For the "make upload" command to succeed, you need to enable 
# the chip's simulation interface with WCHISPTool first. You 
# should then disable it for normal device usage.

CONSOLE_PORT = ttyUSB1

SRCS = \
	main.c

# Toolchain-specific settings ------------------------------------------

SHARED_FLAGS := -march=rv32imac -mabi=ilp32 -mcmodel=medany

CC = ${TOOLCHAIN_PREFIX}-gcc
CFLAGS = ${SDK_INCLUDES} $(SHARED_FLAGS) \
	-msmall-data-limit=8 -ffunction-sections -fdata-sections \
	-fsigned-char -mno-save-restore -Wall

LD = ${TOOLCHAIN_PREFIX}-gcc
# Using: -Wl,--start-group ${SDK_LIBS} -Wl,--end-group
# would be much preferable to: -Wl,--whole-archive ${SDK_LIBS} -Wl,--no-whole-archive
# but for some reason, it doesn't work with this toolchain.
LDFLAGS = $(SHARED_FLAGS) \
	-T${SDK_LDSCRIPT} -nostartfiles -Wl,--gc-sections \
	-Wl,--print-memory-usage -L${SDK_LIBDIR} \
	-Wl,--whole-archive ${SDK_LIBS} -Wl,--no-whole-archive \
	--specs=nano.specs --specs=nosys.specs

OBJCOPY = ${TOOLCHAIN_PREFIX}-objcopy

OBJDUMP = ${TOOLCHAIN_PREFIX}-objdump

SIZE = ${TOOLCHAIN_PREFIX}-size

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -Og -g -DDEBUG=1
	BUILD_DIR = debug
else
	CFLAGS += -Os
	BUILD_DIR = release
endif

BUILD_ROOT = build

OBJDIR = $(BUILD_ROOT)/$(BUILD_DIR)
ARTIFACT_PREFIX = $(OBJDIR)/$(PROJECT_NAME)

DEP_FILE = $(OBJDIR)/dependencies
ELF_FILE = $(ARTIFACT_PREFIX).elf
HEX_FILE = $(ARTIFACT_PREFIX).hex
LST_FILE = $(ARTIFACT_PREFIX).lst
MAP_FILE = $(ARTIFACT_PREFIX).map

OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

# Rules ----------------------------------------------------------------

.PHONY: all clean doc upload console checkenv

all: checkenv $(DEP_FILE) $(ELF_FILE) $(HEX_FILE) $(LST_FILE)


checkenv:
	@/home/tools/bin/check-sdk-env CH573

clean:
	rm -rf $(BUILD_ROOT)

doc:
	doxygen ${TOOLCHAIN_ETC}/doxygen.conf

# For the "make upload" command to succeed, you need to enable 
# the chip's simulation interface with WCHISPTool first. You 
# should then disable it for normal device usage.
upload:
	${SDK_FLASH_TOOL} -f $(patsubst %/openocd, %, ${SDK_FLASH_TOOL})/wch-riscv.cfg -c "program $(ELF_FILE) verify reset exit 0x00000000"

console:
	mate-terminal -e "minicom -b $(CONSOLE_BAUDRATE) -D /dev/$(CONSOLE_PORT)"

-include $(DEP_FILE)

$(DEP_FILE): $(SRCS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -MT $(patsubst %.c, $(OBJDIR)/%.o, $<) -MF /dev/stdout -o /dev/null -c $< >> $(DEP_FILE)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(ELF_FILE): $(OBJS)
	$(LD) $(LDFLAGS) -Wl,-Map,$(MAP_FILE) -o $@ ${SDK_STARTUP} $^
	$(SIZE) --format=berkeley "$(ELF_FILE)"

$(HEX_FILE): $(ELF_FILE)
	$(OBJCOPY) -O ihex $(ELF_FILE) $(HEX_FILE)

$(LST_FILE): $(ELF_FILE)
	$(OBJDUMP) --source --all-headers --demangle --line-numbers --wide $(ELF_FILE) > $(LST_FILE)
