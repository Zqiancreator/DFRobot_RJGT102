# 默认目标
all: help

# 帮助信息
help:
	@echo "Usage: make <target>"
	@echo "Available targets:"
	@echo "  Uno      - Build for Arduino Uno"
	@echo "  ESP32S3  - Build for Arduino ESP32-S3"
	@echo "  CH582    - Build for CH582"

# 根据目标选择编译器和编译选项
ifeq ($(MAKECMDGOALS), Uno)
CC = D:/xiazai/Arduino/hardware/tools/avr/bin/avr-gcc
AR = D:/xiazai/Arduino/hardware/tools/avr/bin/avr-gcc-ar
CFLAGS = -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR
endif

ifeq ($(MAKECMDGOALS), ESP32S3)
CC = D:\xiazai\Arduino\portable\packages\esp32\tools\esp-xs3\2302\bin\xtensa-esp32s3-elf-gcc
AR = D:\xiazai\Arduino\portable\packages\esp32\tools\esp-xs3\2302\bin\xtensa-esp32s3-elf-gcc-ar
CFLAGS = -mlongcalls -ffunction-sections -fdata-sections -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-sign-compare -Wno-enum-conversion -gdwarf-4 -ggdb -freorder-blocks -Wwrite-strings -fstack-protector -fstrict-volatile-bitfields -fno-jump-tables -fno-tree-switch-conversion -std=gnu17 -Wno-old-style-declaration 
endif

ifeq ($(MAKECMDGOALS), CH582)
CC = D:\xiazai\MounRiver_Studio\toolchain\RISC-V Embedded GCC\bin\riscv-none-embed-gcc.exe
AR = D:\xiazai\MounRiver_Studio\toolchain\RISC-V Embedded GCC\bin\riscv-none-embed-ar.exe
CFLAGS = -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g 
endif

INCLUDES = -I./include

OBJS = src/key.o src/rjgt102.o src/rjgt102_security.o src/sha256.o

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libkey.a: $(OBJS)
	$(AR) rcs $@ $^
	copy libkey.a ..\

clean:
	del /Q libkey.a
	del /Q src\*.o
	del /Q src\*.d
	
Uno: libkey.a

ESP32S3: libkey.a

CH582: libkey.a