
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=arm

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : rtc01.gcc.bin rtc01.clang.norm.bin rtc01.clang.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

rtc01.gcc.o : rtc01.c
	$(ARMGNU)-gcc $(COPS) -c rtc01.c -o rtc01.gcc.o

rtc01.gcc.bin : memmap vectors.o rtc01.gcc.o
	$(ARMGNU)-ld -o rtc01.gcc.elf -T memmap vectors.o rtc01.gcc.o
	$(ARMGNU)-objdump -D rtc01.gcc.elf > rtc01.gcc.list
	$(ARMGNU)-objcopy rtc01.gcc.elf rtc01.gcc.bin -O binary

rtc01.clang.bc : rtc01.c
	clang $(LOPS) -c rtc01.c -o rtc01.clang.bc

rtc01.clang.norm.bin : memmap vectors.o rtc01.clang.bc
	llc $(LLCOPS) rtc01.clang.bc -o rtc01.clang.norm.s
	$(ARMGNU)-as rtc01.clang.norm.s -o rtc01.clang.norm.o
	$(ARMGNU)-ld -o rtc01.clang.norm.elf -T memmap vectors.o rtc01.clang.norm.o
	$(ARMGNU)-objdump -D rtc01.clang.norm.elf > rtc01.clang.norm.list
	$(ARMGNU)-objcopy rtc01.clang.norm.elf rtc01.clang.norm.bin -O binary

rtc01.clang.opt.bin : memmap vectors.o rtc01.clang.bc
	opt $(OOPS) rtc01.clang.bc -o rtc01.clang.opt.bc
	llc $(LLCOPS) rtc01.clang.opt.bc -o rtc01.clang.opt.s
	$(ARMGNU)-as rtc01.clang.opt.s -o rtc01.clang.opt.o
	$(ARMGNU)-ld -o rtc01.clang.opt.elf -T memmap vectors.o rtc01.clang.opt.o
	$(ARMGNU)-objdump -D rtc01.clang.opt.elf > rtc01.clang.opt.list
	$(ARMGNU)-objcopy rtc01.clang.opt.elf rtc01.clang.opt.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f uartdiv


