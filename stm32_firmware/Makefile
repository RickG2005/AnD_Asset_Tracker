# Target executable name
TARGET = main

# Toolchain definitions
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# MCU Architecture Flags (STM32F103 uses Cortex-M3)
MCU_FLAGS = -mcpu=cortex-m3 -mthumb

# Compilation and Linker Flags
CFLAGS  = $(MCU_FLAGS) -O0 -g3 -Wall -Iinc -DSTM32F103xB
LDFLAGS = $(MCU_FLAGS) -nostartfiles -Tlinker_script.ld -Wl,-Map=$(TARGET).map

# Source and Object Files tracking
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)

# Default Rule: Build everything
all: build_dir $(TARGET).elf $(TARGET).bin
	@$(SIZE) $(TARGET).elf

# Create a separate build directory for .o files
build_dir:
	@mkdir -p build

# Rule to link the final ELF executable
$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Rule to compile C source files into object files
build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to generate a raw binary (handy for flashing tools)
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Clean up rule to wipe build artifacts
clean:
	rm -rf build $(TARGET).elf $(TARGET).bin $(TARGET).map

# Flash the target using OpenOCD via ST-Link
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(TARGET).elf verify reset exit"

.PHONY: all build_dir clean flash
