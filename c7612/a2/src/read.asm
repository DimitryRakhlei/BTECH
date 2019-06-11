    extern puts
    extern printf
    extern scanf
    extern sprintf
    global main

section .bss
    loop_counter: resb 4

section .text
main:
    _main:
    sub  rsp, 8  


    mov dword[loop_counter], 3   

    .send_loop:        
    xor al, al
    mov rdi, short_upperfmt
    mov rsi, [loop_counter]
    call printf
    
    mov     rax, [loop_counter]
    dec     rax
    cmp     rax, 0
    jz      .send_complete
    mov     [loop_counter], rax
        
    jmp .send_loop

    .send_complete:

    add   rsp, 8 
    ret

clear_reg:
    mov rax, 0
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    xor rcx, rcx
    xor r8,  r8
    ret

message:
    db  "Please Input [ip:port] >" ,0

addrfmt:
    db "%hhu.%hhu.%hhu.%hhu:%hu", 0

ipfmt:
    db "IP as string : %hhu.%hhu.%hhu.%hhu", 10, 0

charfmt:
    db "%hhu", 10, 0

portchars:
    db "port as bytes: %hhu, %hhu", 10, 0

portfmt:
    db "port as int  : %hu", 10, 0

intfmt:
    db "IP as int    : %d", 10, 0

intreadfmt:
    db "%d", 0

    short_upperfmt:
    db "%#06x", 10, 0