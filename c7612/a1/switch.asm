; Author: Dimitry Rakhlei
; Compile with:
;               nasm -f elf switch.asm
;               ld  -m elf_i386 switch.o -o switch
; ALTERNATIVELY:
;               make
;

%define  SYS_EXIT   1
%define  SYS_READ   3
%define  SYS_WRITE  4
%define  STDIN      0
%define  STDOUT     1
%define  STDERR     2
%define	 LF 10
%define  MAX_IN_NUMBER   6
%define  MAX_OUT_NUMBER  10

SECTION .data
    failstr:    db "Input must be between 0 and 65535", 0xa
    failstrl:   equ $-failstr
    num1str:    db "Num1 : "
    num1strl:   equ $-num1str
    num2str:    db "Num2 : "
    num2strl:   equ $-num2str
    num3str:    db "Num3 : "
    num3strl:   equ $-num3str
    nvaluestr:  db "nvalue : "
    nvaluel:    equ $-nvaluestr
    defstr:     db "Default", 0xa
    defstrl:    equ $-defstr
    case0str:   db "Case0: "
    case0strl:  equ $-case0str
    case1str:   db "Case1: "
    case1strl:  equ $-case1str
    case2str:   db "Case2: "
    case2strl:  equ $-case2str
    case3str:   db "Case3: "
    case3strl:  equ $-case3str

SECTION .bss
    out_buffer resb MAX_OUT_NUMBER
    in_buffer  resb MAX_IN_NUMBER
    number     resd 1
    num1       resd 1
    num2       resd 1
    num3       resd 1
    nvalue     resd 1


SECTION .text
;--------------------
global _start
;--------------------
;-------Main---------
;--------------------
_start:
    ; obtaining user inputs
    call GetNum1Input
    call GetNum2Input
    call GetNum3Input
    call GetNvalueInput

    ; starting switch
    ; nvalue increments intitially
    mov eax, [nvalue]
    inc eax
    mov [nvalue], eax
    
    ; case 0 -> cannot happen because nvalue cannot be < 0 
    ; and is incremented at the start
    ; still creating code to satisfy requirements
    cmp eax, 0
    je case_0

    cmp eax, 1
    je case_1
    
    cmp eax, 2
    je case_2
    
    cmp eax, 3
    je case_3
    jmp case_def
;--------------------
;---Switch-Cases-----
;--------------------
case_0:
    mov edx, [num1]
    mov eax, [num2]
    mul edx
    mov [number], eax

    mov ecx, case0str
    mov edx, case0strl
    call print

    mov eax, [number]
    call DisplayN
    jmp exit ; break
case_1:
    mov edx, [num2]
    mov eax, [num3]
    mul edx
    mov [number], eax

    mov ecx, case1str
    mov edx, case1strl
    call print

    mov eax, [number]
    call DisplayN
    jmp exit ; break
case_2:
    mov edx, [num1]
    mov eax, [num3]
    sub eax, edx
    mov [number], eax

    mov ecx, case2str
    mov edx, case2strl
    call print

    mov eax, [number]
    call DisplayN
    jmp exit ; break
case_3:
    mov edx, [num3]
    mov eax, [num1]
    sub eax, edx
    mov [number], eax

    mov ecx, case3str
    mov edx, case3strl
    call print

    mov eax, [number]
    call DisplayN
    jmp exit ; break
case_def:; no break
    mov ecx, defstr
    mov edx, defstrl
    call print
;--------------------
;----Cases-Over------
;--------------------

exit:
    mov eax, SYS_EXIT
    xor ebx, ebx
    int 80h
;--------------------
;----End-of-Main-----
;--------------------

;--------------------
print:
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    int 80H
    ret
;--------------------

;--------------------
Display:
    mov edx, ecx
    mov ecx, esi
    mov ebx, STDOUT
    mov eax, SYS_WRITE
    int 80h
    ret
;--------------------

;--------------------
DisplayN:
    mov esi, out_buffer
    mov ecx, MAX_OUT_NUMBER
    call binasc
    mov byte [esi + ecx], LF
    inc ecx
    call Display
    ret
;--------------------

;--------------------
FailExit:
    ; input for num1
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, failstr
    mov edx, failstrl
    int 80H

    mov eax, SYS_EXIT
    xor ebx, ebx
    int 80h
;--------------------


;--------------------
GetNum1Input:
    ; input for num1
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, num1str
    mov edx, num1strl
    int 80H

    ;read num1
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, in_buffer
    mov edx, MAX_IN_NUMBER
    int 80H
    
    push in_buffer
    call atoi
    add esp, 4
    mov [num1], eax 

    call ValidateNumber

    mov eax, [num1]
    ;call DisplayN ;; uncomment this for debug
    

    ret
;--------------------

;--------------------
GetNum2Input:
    ; input for num2
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, num2str
    mov edx, num2strl
    int 80H

    ;read num2
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, in_buffer
    mov edx, MAX_IN_NUMBER
    int 80H
    

    push in_buffer
    call atoi
    add esp, 4
    mov [num2], eax 

    call ValidateNumber

    mov eax, [num2]
    ;call DisplayN ;; uncomment this for debug
    
    ret
;--------------------

;--------------------
GetNum3Input:
    ; input for num3
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, num3str
    mov edx, num3strl
    int 80H

    ;read num3
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, in_buffer
    mov edx, MAX_IN_NUMBER
    int 80H
    

    push in_buffer
    call atoi
    add esp, 4
    mov [num3], eax

    call ValidateNumber

    mov eax, [num3]
    ;call DisplayN ;; uncomment this for debug
    ret
;--------------------

;--------------------
GetNvalueInput:
    ; input for Nvalue
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, nvaluestr
    mov edx, nvaluel
    int 80H

    ;read Nvalue
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, in_buffer
    mov edx, MAX_IN_NUMBER
    int 80H
    

    push in_buffer
    call atoi
    add esp, 4
    mov [nvalue], eax

    call ValidateNumber

    mov eax, [nvalue]
    ;call DisplayN ;; uncomment this for debug
    ret
;--------------------

;--------------------
ValidateNumber:
    cmp eax, 0
    jl FailExit
    cmp eax, 65535
    jg FailExit
    ret
;--------------------

;--------------------
; Credit: Bryant Keller from nasm.us
atoi:
    push ebx
   
    mov edx, [esp + 8]  ; pointer to string
    xor ebx, ebx ; assume not negative
   
    cmp byte [edx], '-'
    jnz notneg
    inc ebx ; indicate negative
    inc edx ; move past the '-'
notneg:

    xor eax, eax        ; clear "result"
.top:
    movzx ecx, byte [edx]
    inc edx
    cmp ecx, byte '0'
    jb .done
    cmp ecx, byte '9'
    ja .done
   
    ; we have a valid character - multiply
    ; result-so-far by 10, subtract '0'
    ; from the character to convert it to
    ; a number, and add it to result.
   
    lea eax, [eax + eax * 4]
    lea eax, [eax * 2 + ecx - '0']

    jmp short .top
.done:
    test ebx, ebx
    jz notminus
    neg eax
notminus:
    pop ebx
    ret
;------------------------

;--------------------
; Credit: Chuck Crayne - RIP, Chuck.
;convert binary to ascii
;call with	eax = signed binary number
;		esi = address of output string
;		ecx = length of output string
;returns	esi = 1st printed digit
;		ecx = no of digits printed (includes sign if any)
;		other registers preserved
binasc:	push	edx
	push	ebx
	push	edi
	push	eax
	mov	edi,esi		        ;save start of string
ba1:	mov	byte [esi],' '	;fill string with blanks
	inc	esi
	loop	ba1
	mov	ebx,10		        ;initialize divisor
	or	eax,eax		        ;value negative?
	jns	ba2	    	        ;no problem
	neg	eax		            ;make it positive
ba2:	xor	edx,edx	        ;clear high part of dividend
	div	ebx		            ;divide by 10
	add	dl,'0'		        ;convert to ascii digit
	dec	esi		            ;step backwards through buffer
	mov	[esi],dl	        ;store digit
	inc	ecx
	cmp	esi,edi		        ;out of space
	jz	ba4		            ;yes - quit
	or	eax,eax		        ;all digits printed?
	jnz	ba2		            ;no - keep trucking
	pop	eax		            ;get original value
	or	eax,eax		        ;negative?
	jns	ba3		            ;no - quit
	dec	esi		            ;place for sign
	mov	byte [esi],'-'
	inc	ecx		            ;add to char count
ba3:	pop	edi
	pop	ebx
	pop	edx
	ret
ba4:	pop	eax
	jmp	ba3
;------------------- 
