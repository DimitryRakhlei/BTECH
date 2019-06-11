section     .data
    msg     db "test", 0xa
    msglen  equ $-msg

section     .bss
    a       resb 5
    b       resb 5
    c       resb 5

section     .text
    global  _start                              

print:
    mov     ebx, 1
    mov     eax, 4
    int     0x80
    ret

read:
    mov     eax, 3 
	mov     ebx, 0     ; descriptor value for stdin
    int     80h 
    ret

prt_a:
    mov     ecx, a
    mov     edx, 5                             
    call    print
    ret

prt_b:
    mov     ecx, b
    mov     edx, 5                             
    call    print
    ret

prt_c:
    mov     ecx, c
    mov     edx, 5                             
    call    print
    ret

prt_test:
    mov ecx, msg
    mov edx, msglen
    call     print
    ret

_start:                                         
main:
    ; get input for a
    mov     ecx, a
    mov     edx, 5
    call    read

    ; get input for b
    mov     ecx, b
    mov     edx, 5
    call    read

    ; get input for c
    mov     ecx, c
    mov     edx, 5
    call    read

    ; -----------
    ; DONE INPUTS
    ; -----------


    ; if a < b
    mov     ebx, [a]
    mov     eax, [b]
    cmp     ebx, eax
    jl      alb
    jge     agb
        ; TRUE
        alb: ; a is less than b  
        mov     ebx, [a]
        mov     eax, [c]
        cmp     ebx, eax
        jl      alb_alc
        jge     alb_agc

            alb_alc: ; a < b && a < c
                mov     ebx, [b]
                mov     eax, [c]
                cmp     ebx, eax
                jl      alb_alc_blc
                jge     alb_alc_bgc
                    alb_alc_blc: ; (a < b && a < c) && (b < c)
                    call    prt_c
                    call    prt_b
                    call    prt_a
                    jmp     end
                    alb_alc_bgc: ; (a < b && a < c) && (b > c)
                    call    prt_b
                    call    prt_c
                    call    prt_a
                    jmp     end
            jmp     end
            alb_agc: ; a < b && a > c
            call prt_b
            call prt_a
            call prt_c
            jmp     end

        jmp     end ; make sure to have no fallthrough

        ; ELSE
        agb: ; b is less than a
        mov     ebx, [b]
        mov     eax, [c]
        cmp     ebx, eax
        jl      agb_blc
        jge     agb_bgc
            agb_blc:
            mov     ebx, [a]
            mov     eax, [c]
            cmp     ebx, eax
            jl      agb_blc_alc
            jge     agb_blc_agc
            jmp     end

                agb_blc_alc:
                call    prt_c
                call    prt_a
                call    prt_b
                jmp     end
                agb_blc_agc:
                call    prt_a
                call    prt_c
                call    prt_b
                jmp     end
            jmp     end
            agb_bgc:
            call    prt_a
            call    prt_b
            call    prt_c
            jmp     end
        jmp     end ; make sure to have no fallthrough
    end:
    mov     eax,1                              
    int     0x80                                

 
