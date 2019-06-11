; Assemble as: nasm -f elf -g readwrite.asm
; Link as: ld -m elf_i386 -o readwrite readwrite.o

section .data ;Data segment 
	InputNUm db 'Enter a number: ' ;Ask the user to enter a number 
	StrLen equ $-InputNUm          ;The length of the message 
	DispNum db 'Value Entered: ' 
	DispNumLen equ $-DispNum 

section .bss    ;Uninitialized data 
	num resb 5 

section .text   ;Code Segment 
global _start 
_start: 
	;Prompt User 
	mov eax, 4 
	mov ebx, 1     ; descriptor value for stdout
	mov ecx, InputNUm 
	mov edx, StrLen 
	int 80h 

	;Read and store the user input 
	mov eax, 3 
	mov ebx, 0     ; descriptor value for stdin
	mov ecx, num 
	mov edx, 5     ;5 bytes (numeric, 1 for sign) of that information 
	int 80h 

	;Output the message 'Value Entered: ' 
	mov eax, 4 
	mov ebx, 1 
	mov ecx, DispNum 
	mov edx, DispNumLen 
	int 80h 

	;Output the number entered 
	mov eax, 4 
	mov ebx, 1 
	mov ecx, num 
	mov edx, 5 
	int 80h 

; Exit code 
	mov eax, 1 
	mov ebx, 0 
	int 80h 

