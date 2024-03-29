;; TCP echo server using x86_64 Linux syscalls
;; Assemble and link as follows:
;;        nasm -f elf64 -o server.o server.asm
;;        ld server.o -o server
;;   
;;

global _start

;; Data definitions
struc sockaddr_in
    .sin_family resw 1
    .sin_port resw 1
    .sin_addr resd 1
    .sin_zero resb 8
endstruc

section .bss
    sock resw 2
    client resw 2
    echobuf resb 256
    read_count resw 2

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

    ;; sockaddr_in structure for the address the listening socket binds to
    pop_sa istruc sockaddr_in ; 0x100007F = 127.0.0.1
        at sockaddr_in.sin_family, dw 2           ; AF_INET
        at sockaddr_in.sin_port, dw 0xbc1d        ; port 7612 in host byte order
        at sockaddr_in.sin_addr, dd 0x100007F             ; localhost - INADDR_ANY
        at sockaddr_in.sin_zero, dd 0, 0
    iend
    sockaddr_in_len     equ $ - pop_sa

section .text

;; Sever main entry point
_start:
    ;; Initialize listening and client socket values to 0, used for cleanup 
    mov      word [sock], 0
    mov      word [client], 0

    ;; Initialize socket
    call     _socket

    ;; Bind and Listen
    call     _listen

    ;; Main loop handles connection requests (accept()) then echoes data back to client
    .mainloop:
        call     _accept

        ;; Read and echo string back to the client
        ;; up the connection on their end.
        .readloop:
            call     _read
            call     _echo

            ;; read_count is set to zero when client hangs up
            mov     rax, [read_count]
            cmp     rax, 0
            je      .read_complete
        jmp .readloop

        .read_complete:
        ;; Close client socket
        mov    rdi, [client]
        call   _close_sock
        mov    word [client], 0
    jmp    .mainloop

    ;; Exit with success (return 0)
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

;; Calls sys_bind and sys_listen to start listening for connections
_listen:
    mov        rax, 49                  ; SYS_BIND
    mov        rdi, [sock]              ; listening socket fd
    mov        rsi, pop_sa              ; sockaddr_in struct
    mov        rdx, sockaddr_in_len     ; length of sockaddr_in
    syscall

    ;; Check call succeeded
    cmp        rax, 0
    jl         _bind_fail

    ;; Bind succeeded, call sys_listen
    mov        rax, 50          ; SYS_LISTEN
    mov        rsi, 5           ; backlog 
    syscall

    ;; Check for success
    cmp        rax, 0
    jl         _listen_fail

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
_read:
    ;; Call sys_read
    mov     rax, 0          ; SYS_READ
    mov     rdi, [client]   ; client socket fd
    mov     rsi, echobuf    ; buffer
    mov     rdx, 256        ; read 256 bytes 
    syscall 

    ;; Copy number of bytes read to variable
    mov     [read_count], rax

    ret 

;; Sends up to the value of read_count bytes from echobuf to the client socket
;; using sys_write 
_echo:
    mov     rax, 1               ; SYS_WRITE
    mov     rdi, [client]        ; client socket fd
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