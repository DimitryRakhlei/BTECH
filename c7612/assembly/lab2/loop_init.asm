; simple loop example which iterates through a loop ten times, 
; printing the variable value each time. 

section	.bss
    nums resb 10

section .data
    newLine db 0xa,0xd
    NewLen equ $- newLine

section	.text
    global _start   
	
_start:	            
    mov ecx,10      ; loop variable -> counter
    mov eax, '0'    ; initial variable value
    lea edi, [nums] ; edi = nums[0]
	
REPEAT:
    push ecx        ; save the loop variable on stack before printing
    mov [edi], eax   
    mov eax, 4
    mov ebx, 1
    mov ecx, edi       
    mov edx, 1        
    int 0x80
		
    mov eax, [edi]  ; start the next iteration
    inc edi
    inc eax         ; increment variable 
    pop ecx         ; retrieve the loop vaiable from stack
    loop REPEAT     ; execute loop - decrement ecx (counter)

    mov eax, 4
    mov ebx, 1
    mov ecx, newLine
    mov edx, NewLen
    int 0x80
    mov ecx,10
    lea edi, [nums]
REPEAT2:
    push ecx
    mov eax, 4
    mov ebx, 1
    mov ecx, edi
    mov edx, 1
    int 0x80

    inc edi
    pop ecx
    loop REPEAT2

    ; print newline
    mov edx, NewLen     ; msg length
    mov ecx, newLine
    mov ebx, 1          ; stdout
    mov eax, 4          ; write
    int 0x80
  
    mov eax,1           ;system call number (sys_exit)
    int 0x80            ;call kernel

