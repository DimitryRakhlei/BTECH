;; TCP echo server using x86_64 Linux syscalls
;; Assemble and link as follows:
;;        nasm -f elf64 -o server.o server.asm
;;        ld server.o -lc -o server
;;   
;;

;global _start
global main
extern puts
extern printf
extern scanf
extern sprintf


;; Data definitions
struc sockaddr_in
    .sin_family resw 1
    .sin_port resw 1
    .sin_addr resd 1
    .sin_zero resb 8
endstruc

section .bss
    netstr      resb 128
    sock        resw 2
    client      resw 2
    echobuf     resb 256
    read_count  resw 2
    
    loop_counter: resb 4

    tempb:   resb 1
    ;hord order port bytes as per input
    p0:     resb 1
    p1:     resb 1
    ;host order ip bytes  as per input
    b0:     resb 1
    b1:     resb 1
    b2:     resb 1
    b3:     resb 1
    ;network order
    n0:     resb 1
    n1:     resb 1
    n2:     resb 1
    n3:     resb 1     



section .data
    sock_err_msg        db "Failed to initialize socket", 0x0a, 0
    sock_err_msg_len    equ $ - sock_err_msg

    bind_err_msg        db "Failed to bind socket", 0x0a, 0
    bind_err_msg_len    equ $ - bind_err_msg

    lstn_err_msg        db "Socket Listen Failed", 0x0a, 0
    lstn_err_msg_len    equ $ - lstn_err_msg

    accept_err_msg      db "Accept Failed", 0x0a, 0
    accept_err_msg_len  equ $ - accept_err_msg

    accept_msg          db "Client Connected!", 0x0a, 0
    accept_msg_len      equ $ - accept_msg

    connect_err_msg          db "Connect Failed", 0x0a, 0
    connect_err_msg_len      equ $ - connect_err_msg

    loop_msg            db "Left:", 0
    loop_msg_len        equ $ - loop_msg

    newline             db "", 0, 0x0a
    newline_len         equ $ - newline


    ;; sockaddr_in structure for the address the sending socket binds to
    pop_sa istruc sockaddr_in
        at sockaddr_in.sin_family, dw 2           ; AF_INET
        at sockaddr_in.sin_port, dw 0             ; port 0 in host byte order
        at sockaddr_in.sin_addr, dd 0             ; localhost - INADDR_ANY
        at sockaddr_in.sin_zero, dd 0, 0
    iend
    pop_sockaddr_in_len     equ $ - pop_sa

    ;; sockaddr_in structure for the address the destination socket binds to
    remote_sa istruc sockaddr_in 
        at sockaddr_in.sin_family, dw 2           ; AF_INET
        at sockaddr_in.sin_port, dw 0;0xbc1d        ; port 7512 in host byte order
        at sockaddr_in.sin_addr, dd 0              ; 0x1800A8C0    ; 192.168.0.24
        at sockaddr_in.sin_zero, dd 0, 0
    iend
    remote_sockaddr_in_len     equ $ - remote_sa

section .text

;; Sever main entry point
main:
    _main:
    sub  rsp, 8  
    ;; Initialize listening and client socket values to 0, used for cleanup 
    mov      word [sock], 0
    mov      word [client], 0

    xor     al, al
    mov     rdi, message
    call    printf

    call    clear_reg
    xor     al, al
    mov     rdi, addrfmt
    mov     rsi, b0
    mov     rdx, b1
    mov     rcx, b2
    mov     r8,  b3
    mov     r9,  p0
    call    scanf

    ; print port
    call    clear_reg
    xor     al, al
    mov     rdi, portfmt
    mov     rsi, [p0]
    call    printf
    
    ; invert bytes
    mov     al , [p1]
    mov     [remote_sa + sockaddr_in.sin_port], al
    mov     al, [p0]
    mov     [remote_sa + sockaddr_in.sin_port + 1], al

    ; print port bytes
    call    clear_reg
    xor     al, al
    mov     rdi, portchars
    mov     rsi, [remote_sa + sockaddr_in.sin_port]
    mov     rdx, [remote_sa + sockaddr_in.sin_port + 1]
    call    printf

    ; print port in network byte order
    call    clear_reg
    xor     al, al
    mov     rdi, portfmt
    mov     rsi, [remote_sa + sockaddr_in.sin_port]
    call    printf

    ; convert to network byte order
    mov     al, [b0]
    mov     [remote_sa + sockaddr_in.sin_addr], al
    mov     al, [b1]
    mov     [remote_sa + sockaddr_in.sin_addr + 1], al
    mov     al, [b2]
    mov     [remote_sa + sockaddr_in.sin_addr + 2], al
    mov     al, [b3]
    mov     [remote_sa + sockaddr_in.sin_addr + 3], al

    ; print ip in host order
    call    clear_reg
    xor     al, al
    mov     rdi, intfmt
    mov     rsi, [remote_sa + sockaddr_in.sin_addr]
    call    printf

    ;; Initialize socket
     call     _socket

    ;; Bind and Listen
    call     _bind

    ;; Connect to remote
    call    _connect


    mov dword[loop_counter], 5 

    xor al, al
    call clear_reg
    mov rdi, print_counter
    mov rsi, [loop_counter]
    call printf
    ;; Send strings to the server
    .send_loop:
        call     _read_input
        call     _send
        call     _read_response

        mov     ax, [loop_counter]
        inc     ax
        cmp     ax, 0
        jle      .send_complete
        
        mov     [loop_counter], ax
        
        xor al, al
        mov rdi, print_counter
        mov rsi, [loop_counter]
        call printf
        
        
    jmp .send_loop

    .send_complete:

    ;; Exit with success (return 0)
    add     rsp, 8 
    mov     rdi, 0
    call     _exit



;; Performs a sys_socket call to initialise a TCP/IP listening socket. 
;; Stores the socket file descriptor in the sock variable
_socket:
    mov         rax, 41     ; SYS_SOCKET
    mov         rdi, 2      ; AF_INET
    mov         rsi, 1      ; SOCK_STREAM
    mov         rdx, 0    
    syscall
    
    ;; Check if socket was created successfully
    cmp        rax, 0
    jle        _socket_fail

    ;; Store the new socket descriptor 
    mov        [sock], rax

    ret




;; Calls sys_bind
_bind:
    mov        rax, 49                  ; SYS_BIND
    mov        rdi, [sock]              ; listening socket fd
    mov        rsi, pop_sa              ; sockaddr_in struct
    mov        rdx, pop_sockaddr_in_len ; length of sockaddr_in
    syscall

    ;; Check call succeeded
    cmp        rax, 0
    jl         _bind_fail

    
    ret

;; Connect to remote
_connect:
    ;; Call sys_connect
    mov     rax, 42
    mov     rdi, [sock]
    mov     rsi, remote_sa
    mov     rdx, remote_sockaddr_in_len
    syscall

    ;; Check if call succeeded
    cmp       rax, 0
    jl        _connect_fail

    ;; Print connection message to stdout
    mov       rax, 1             ; SYS_WRITE
    mov       rdi, 1             ; STDOUT
    mov       rsi, accept_msg
    mov       rdx, accept_msg_len
    syscall

    ret

;; Accept a cleint connection and store the new client socket descriptor
_accept:
    ;; Call sys_accept
    mov       rax, 43         ; SYS_ACCEPT
    mov       rdi, [sock]     ; listening socket fd
    mov       rsi, 0          ; NULL sockaddr_in value as we don't need that data
    mov       rdx, 0          ; NULLs have length 0
    syscall

    ;; Check if call succeeded
    cmp       rax, 0
    jl        _accept_fail

    ;; Store returned client socket descriptor 
    mov     [client], rax

    ;; Print connection message to stdout
    mov       rax, 1             ; SYS_WRITE
    mov       rdi, 1             ; STDOUT
    mov       rsi, accept_msg
    mov       rdx, accept_msg_len
    syscall

    ret

;; Reads up to 256 bytes from the client into echobuf and sets the read_count variable
;; to be the number of bytes read by sys_read
_read_input:
    mov rax, 0  
    mov rdi, 0
    mov rsi, echobuf
    mov rdx, 256
    syscall

    ; ;; Call sys_read
    ; mov     rax, 0          ; SYS_READ
    ; mov     rdi, [client]   ; client socket fd
    ; mov     rsi, echobuf    ; buffer
    ; mov     rdx, 256        ; read 256 bytes 
    ; syscall 

    ;; Copy number of bytes read to variable
    mov     [read_count], rax

    ret

_read_response:
    ;; Call sys_read
    mov     rax, 0          ; SYS_READ
    mov     rdi, [sock]   ; client socket fd
    mov     rsi, echobuf    ; buffer
    mov     rdx, 256        ; read 256 bytes 
    syscall

    mov     rax, 1               ; SYS_WRITE
    mov     rdi, 1        ; client socket fd
    mov     rsi, echobuf         ; buffer
    mov     rdx, [read_count]    ; number of bytes received in _read
    syscall

    ; Copy number of bytes read to variable
    mov     [read_count], rax

    ret

;; Sends up to the value of read_count bytes from echobuf to the client socket
;; using sys_write 
_send:
    mov     rax, 1               ; SYS_WRITE
    mov     rdi, [sock]        ; client socket fd
    mov     rsi, echobuf         ; buffer
    mov     rdx, [read_count]    ; number of bytes received in _read
    syscall

    ret

;; Performs sys_close on the socket in rdi
_close_sock:
    mov     rax, 3        ; SYS_CLOSE
    syscall

    ret

;; Error Handling code
;; _*_fail loads the rsi and rdx registers with the appropriate
;; error messages for given system call. Then call _fail to display the
;; error message and exit the application.
_socket_fail:
    mov     rsi, sock_err_msg
    mov     rdx, sock_err_msg_len
    call    _fail

_connect_fail:
    mov     rsi, connect_err_msg
    mov     rdx, connect_err_msg_len
    call    _fail

_bind_fail:
    mov     rsi, bind_err_msg
    mov     rdx, bind_err_msg_len
    call    _fail

_listen_fail:
    mov     rsi, lstn_err_msg
    mov     rdx, lstn_err_msg_len
    call    _fail

_accept_fail:
    mov     rsi, accept_err_msg
    mov     rdx, accept_err_msg_len
    call    _fail

;; Calls the sys_write syscall, writing an error message to stderr, then exits
;; the application. rsi and rdx must be loaded with the error message and
;; length of the error message before calling _fail
_fail:
    mov        rax, 1 ; SYS_WRITE
    mov        rdi, 2 ; STDERR
    syscall

    mov        rdi, 1
    call       _exit


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


send_timesfmt:
    db "Loop count: %d", 10, 0

short_upperfmt:
    db "%#08x", 10, 0

print_counter:
    db "Counter: %d", 10, 0

clear_reg:
    mov rax, 0
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    xor rcx, rcx
    xor r8,  r8
    ret


;; Exits cleanly, checking if the listening or client sockets need to be closed
;; before calling sys_exit
_exit:
    mov        rax, [sock]
    cmp        rax, 0
    je         .client_check
    mov        rdi, [sock]
    call       _close_sock

    .client_check:
    mov        rax, [client]
    cmp        rax, 0
    je         .perform_exit
    mov        rdi, [client]
    call       _close_sock

    .perform_exit:
    mov        rax, 60
    syscall