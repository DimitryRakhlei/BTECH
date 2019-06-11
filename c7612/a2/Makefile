all:
	nasm -f elf64 -g -o bin/server.o src/server.asm
	ld -g bin/server.o -o bin/server

	nasm -f elf64 -g -o bin/client.o src/client.asm
	gcc -m64 -g bin/client.o  -o bin/client

	nasm -f elf64 -g -o bin/read.o src/read.asm
	gcc -m64 -g bin/read.o  -o bin/read

clean:
	rm bin/*
