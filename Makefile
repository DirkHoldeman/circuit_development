TARGET=circuit_development

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	arm-none-eabi-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

$(TARGET).elf: main.o functions.o sysinit.o crt0.o
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -nostdlib -nostartfiles -Os --specs=nosys.specs -T/usr/lib/arm-none-eabi/ldscripts/Teensy31_flash.ld -o $(TARGET).elf $^

%.o: %.c
	arm-none-eabi-gcc -Wall -mcpu=cortex-m4 -mthumb -DF_CPU=48000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -std=c99 -c $^ -o $@

%.o: %.s
	arm-none-eabi-gcc -c $^ -o $@

install:
	teensy

clean:
	rm -f *.o $(TARGET).elf $(TARGET).hex
