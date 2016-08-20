
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

all : ssd1306b.gcc.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ssd1306b.gcc.o : ssd1306b.c 
	$(ARMGNU)-gcc $(COPS) -c ssd1306b.c -o ssd1306b.gcc.o

ssd1306b.gcc.bin : memmap vectors.o ssd1306b.gcc.o
	$(ARMGNU)-ld -o ssd1306b.gcc.elf -T memmap vectors.o ssd1306b.gcc.o
	$(ARMGNU)-objdump -D ssd1306b.gcc.elf > ssd1306b.gcc.list
	$(ARMGNU)-objcopy ssd1306b.gcc.elf ssd1306b.gcc.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f uartdiv


