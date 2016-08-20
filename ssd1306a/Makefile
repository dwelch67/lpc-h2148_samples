
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=arm

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : ssd1306a.gcc.bin 

llvm : ssd1306a.clang.norm.bin ssd1306a.clang.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ssd1306a.gcc.o : ssd1306a.c fontdata.h
	$(ARMGNU)-gcc $(COPS) -c ssd1306a.c -o ssd1306a.gcc.o

ssd1306a.gcc.bin : memmap vectors.o ssd1306a.gcc.o
	$(ARMGNU)-ld -o ssd1306a.gcc.elf -T memmap vectors.o ssd1306a.gcc.o
	$(ARMGNU)-objdump -D ssd1306a.gcc.elf > ssd1306a.gcc.list
	$(ARMGNU)-objcopy ssd1306a.gcc.elf ssd1306a.gcc.bin -O binary

ssd1306a.clang.bc : ssd1306a.c
	clang $(LOPS) -c ssd1306a.c -o ssd1306a.clang.bc

ssd1306a.clang.norm.bin : memmap vectors.o ssd1306a.clang.bc
	llc $(LLCOPS) ssd1306a.clang.bc -o ssd1306a.clang.norm.s
	$(ARMGNU)-as ssd1306a.clang.norm.s -o ssd1306a.clang.norm.o
	$(ARMGNU)-ld -o ssd1306a.clang.norm.elf -T memmap vectors.o ssd1306a.clang.norm.o
	$(ARMGNU)-objdump -D ssd1306a.clang.norm.elf > ssd1306a.clang.norm.list
	$(ARMGNU)-objcopy ssd1306a.clang.norm.elf ssd1306a.clang.norm.bin -O binary

ssd1306a.clang.opt.bin : memmap vectors.o ssd1306a.clang.bc
	opt $(OOPS) ssd1306a.clang.bc -o ssd1306a.clang.opt.bc
	llc $(LLCOPS) ssd1306a.clang.opt.bc -o ssd1306a.clang.opt.s
	$(ARMGNU)-as ssd1306a.clang.opt.s -o ssd1306a.clang.opt.o
	$(ARMGNU)-ld -o ssd1306a.clang.opt.elf -T memmap vectors.o ssd1306a.clang.opt.o
	$(ARMGNU)-objdump -D ssd1306a.clang.opt.elf > ssd1306a.clang.opt.list
	$(ARMGNU)-objcopy ssd1306a.clang.opt.elf ssd1306a.clang.opt.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f uartdiv


