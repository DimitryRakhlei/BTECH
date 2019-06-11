; Example of using the CMP and JGE instructions
; Assemble as: nasm -f elf -g cmptst.asm
; Link as: ld -m elf_i386 -o cmptst cmptst.o

section .data
    num1 dd '10'
    num2 dd '30'
    msg1 db "ebx > eax", 0xa
    msg2 db "ebx < eax", 0xa
    MsgLen equ $-msg2 ;The length of the message 

section .text
global _start
_start:
   nop
   mov eax, [num1]
   mov ebx, [num2]
   cmp ebx, eax ; test to see if ebx > eax
   jge greater  ; jump to "greater" if true
   mov edx, MsgLen  ; msg length
   mov ecx, msg2
   mov ebx, 1   ; stdout
   mov eax, 4   ; write
   int 0x80
   mov eax, 1   ;exit
   int 0x80
greater:
   mov edx, MsgLen
   mov ecx, msg1
   mov ebx, 1   ; stdout
   mov eax, 4   ; write
   int 0x80
   mov eax, 1   ;exit
   int 0x80

    

