; interrupt number 0x80 is used to perform write to stdout
; the venerable Hello World program
; Assemble as: nasm -f elf -g hello.asm
; Link as: ld -m elf_i386 -o hello hello.o

section .text 
global _start  
_start: ;program entry point 
    mov edx,len ;message length 
    mov ecx,msg ;message to write 
    mov ebx,1 ;file descriptor (stdout) 
    mov eax,4 ;system call number (sys_write) 
    int 0x80 ;call kernel

    mov eax,1 ;system call number (sys_exit) 
    int 0x80 ;call kernel 

section .data 
msg db 'Hello World!', 0xa ; string + newline char 0xa
len equ $ - msg ;length of the string 

