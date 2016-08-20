
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=arm

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : gps_clock02.gcc.bin 

llvm : gps_clock02.clang.norm.bin gps_clock02.clang.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

gps_clock02.gcc.o : gps_clock02.c
	$(ARMGNU)-gcc $(COPS) -c gps_clock02.c -o gps_clock02.gcc.o

gps_clock02.gcc.bin : memmap vectors.o gps_clock02.gcc.o
	$(ARMGNU)-ld -o gps_clock02.gcc.elf -T memmap vectors.o gps_clock02.gcc.o
	$(ARMGNU)-objdump -D gps_clock02.gcc.elf > gps_clock02.gcc.list
	$(ARMGNU)-objcopy gps_clock02.gcc.elf gps_clock02.gcc.bin -O binary

gps_clock02.clang.bc : gps_clock02.c
	clang $(LOPS) -c gps_clock02.c -o gps_clock02.clang.bc

gps_clock02.clang.norm.bin : memmap vectors.o gps_clock02.clang.bc
	llc $(LLCOPS) gps_clock02.clang.bc -o gps_clock02.clang.norm.s
	$(ARMGNU)-as gps_clock02.clang.norm.s -o gps_clock02.clang.norm.o
	$(ARMGNU)-ld -o gps_clock02.clang.norm.elf -T memmap vectors.o gps_clock02.clang.norm.o
	$(ARMGNU)-objdump -D gps_clock02.clang.norm.elf > gps_clock02.clang.norm.list
	$(ARMGNU)-objcopy gps_clock02.clang.norm.elf gps_clock02.clang.norm.bin -O binary

gps_clock02.clang.opt.bin : memmap vectors.o gps_clock02.clang.bc
	opt $(OOPS) gps_clock02.clang.bc -o gps_clock02.clang.opt.bc
	llc $(LLCOPS) gps_clock02.clang.opt.bc -o gps_clock02.clang.opt.s
	$(ARMGNU)-as gps_clock02.clang.opt.s -o gps_clock02.clang.opt.o
	$(ARMGNU)-ld -o gps_clock02.clang.opt.elf -T memmap vectors.o gps_clock02.clang.opt.o
	$(ARMGNU)-objdump -D gps_clock02.clang.opt.elf > gps_clock02.clang.opt.list
	$(ARMGNU)-objcopy gps_clock02.clang.opt.elf gps_clock02.clang.opt.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f uartdiv


