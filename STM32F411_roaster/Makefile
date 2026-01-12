CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
				 -O0 -g -Wall \
				 -Isrc \
				 -Iinclude \
				 -ISTM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
				 -ISTM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include

LDFLAGS = -T linker.ld -nostartfiles -Wl,--gc-sections

SRCS = src/main.c startup_stm32f411.s src/GPIO.c src/lcd.c

TARGET = blink.elf

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

flash:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
		-c "program $(TARGET) verify reset exit"

clean:
	rm -f $(TARGET) *.o *.bin
