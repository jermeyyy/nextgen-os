LDSCRIPT = link.ld
#BINPATH	 = /usr/local/cross/bin/
BINPATH	 = /cygdrive/c/Users/Jermey/opt/cross/bin/
BOOTSECT = bin/base.img
COPYDISK = cat
CC	= @gcc
#CC       = $(BINPATH)i586-elf-gcc
SRC	 = src/
CFLAGS   = -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -pipe -fleading-underscore -nostdinc -std=c99 -nostdlib -fno-stack-protector -fno-builtin -I./include -c
LD       = @$(BINPATH)i686-elf-ld
LDFLAGS	 = -T link.ld -o build/kernel.bin build/start.o build/isrs.o build/idt.o build/irq.o build/main.o build/scrn.o build/kb.o build/win_mngr.o build/gdt.o build/timer.o build/rtc.o build/stdlib.o build/ordered_array.o build/kheap.o build/fs.o build/initrd.o build/paging.o build/cpuid_asm.o build/cpuid.o build/shell.o
AS       = @nasm
ASFLAGS  = -f elf
CDMAKE	 = mkisofs -b nextgen.img -o build/nextgen.iso build/

all: clean ver initrd kernel.bin kernel iso

kernel: kernel.bin
	@cp -u bin/base.img build/nextgen.img
	@mcopy -i build/nextgen.img build/initrd.img build/kernel.bin ::

iso: kernel
	@rm -rf build/nextgen.iso
	@$(CDMAKE)

ver:
	@date +%-m%0d%g%0H%0M > date;  
	@cat head date > src/date.h

initrd:
	#bin/make_initrd bin/test1.txt 1 bin/test2.txt 2 bin/test3.txt 3 bin/test4.txt 4
	bin/make_initrd.exe bin/test1.txt 1 bin/test2.txt 2 bin/test3.txt 3 bin/test4.txt 4
	@cp -fr bin/initrd.img build/initrd.img
	
clean:
	@rm -rf build/* src/date.h

start.o: $(SRC)start.asm
	$(AS) $(ASFLAGS) -o build/start.o $(SRC)start.asm
	
cpuid_asm.o: $(SRC)cpuid_asm.asm
	$(AS) $(ASFLAGS) -o build/cpuid_asm.o $(SRC)cpuid_asm.asm

main.o: $(SRC)main.c
	$(CC) $(CFLAGS) -o build/main.o $(SRC)main.c

scrn.o: $(SRC)scrn.c
	$(CC) $(CFLAGS) -o build/scrn.o $(SRC)scrn.c

win_mngr.o: $(SRC)win_mngr.c
	$(CC) $(CFLAGS) -o build/win_mngr.o $(SRC)win_mngr.c	

gdt.o: $(SRC)gdt.c
	$(CC) $(CFLAGS) -o build/gdt.o $(SRC)gdt.c
	
idt.o: $(SRC)idt.c
	$(CC) $(CFLAGS) -o build/idt.o $(SRC)idt.c	

isrs.o: $(SRC)isrs.c
	$(CC) $(CFLAGS) -o build/isrs.o $(SRC)isrs.c
	
irq.o: $(SRC)irq.c
	$(CC) $(CFLAGS) -o build/irq.o $(SRC)irq.c

timer.o: $(SRC)timer.c
	$(CC) $(CFLAGS) -o build/timer.o $(SRC)timer.c

kb.o: $(SRC)kb.c
	$(CC) $(CFLAGS) -o build/kb.o $(SRC)kb.c

rtc.o: $(SRC)rtc.c
	$(CC) $(CFLAGS) -o build/rtc.o $(SRC)rtc.c

stdlib.o: $(SRC)stdlib.c
	$(CC) $(CFLAGS) -o build/stdlib.o $(SRC)stdlib.c
	
kheap.o: $(SRC)kheap.c
	$(CC) $(CFLAGS) -o build/kheap.o $(SRC)kheap.c
	
ordered_array.o: $(SRC)ordered_array.c
	$(CC) $(CFLAGS) -o build/ordered_array.o $(SRC)ordered_array.c
	
fs.o: $(SRC)fs.c
	$(CC) $(CFLAGS) -o build/fs.o $(SRC)fs.c
	
initrd.o: $(SRC)initrd.c
	$(CC) $(CFLAGS) -o build/initrd.o $(SRC)initrd.c
	
paging.o: $(SRC)paging.c
	$(CC) $(CFLAGS) -o build/paging.o $(SRC)paging.c

cpuid.o: $(SRC)cpuid.c
	$(CC) $(CFLAGS) -o build/cpuid.o $(SRC)cpuid.c	

shell.o: $(SRC)shell.c
	$(CC) $(CFLAGS) -o build/shell.o $(SRC)shell.c
	
kernel.bin: main.o start.o scrn.o kb.o win_mngr.o isrs.o gdt.o idt.o irq.o timer.o rtc.o stdlib.o ordered_array.o kheap.o fs.o initrd.o paging.o cpuid_asm.o cpuid.o shell.o
	$(LD) $(LDFLAGS) $(OBJ)
