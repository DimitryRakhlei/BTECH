all: switch.asm
	nasm -f elf switch.asm
	ld  -m elf_i386 switch.o -o switch

clean: switch.o switch
	rm switch.o
	rm switch
