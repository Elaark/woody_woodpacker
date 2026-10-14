section .text
global main
extern _decrypt

; MAP_ANONYMOUS        equ        1
O_RDONLY        equ        00

_error_exit:
        mov rax, 0x3c         ; syscall number 60 for exit
        mov rdi, 1            ; error only being 1
        syscall
        ret
        
_close_fd:
        mov rdi, rax
        mov rax, 0x03
        syscall
        ret
        
main:
        mov rax, 0x13f        ; syscall number 319 for memfd_create
        mov rdi, filename     ; filename
        xor rsi, rsi          ; Anonymous by default
        syscall               ; memfd_create

        cmp rax, -1
        je _memfd_create_error
        
        ; now I have a fd inside memory (step 1) -> must put my host in it (step 2)

        ; must get my own fd to to the send
        push rax              ; save in stack the new fd

        mov rax, 0x02         ; syscal number 5 for open
        mov rdi, own_path     ; name to find my own fd
        mov rsi, O_RDONLY     ; Read-only flag
        xor rdx, rdx          ; no mode
        syscall

        cmp rax, -1
        je _open_error
        push rax
        
        mov rdi, rax
        mov rax, 0x08         ; syscall number 8 for lseek
        mov rsi, stub_ending  ; where the stub end, value the protector MUST SET
        xor rdx, rdx          ; just like SEEK_SET
        syscall

        cmp rax, -1
        je _lseek_error
        
        mov rsi, rax          ; return of lseek
        mov rax, 0x27         ; syscall number 40 for sendfile
        mov rdi, rsp[1]       ; get from stack the new fd, fd_out
        mov rsi, rsp          ; this must be fd in, 
       
_normal_exit:
        pop rax
        call _close_fd
        pop rax
        call _close_fd
        ret
        
_memfd_create_error:
        jmp _error_exit

_open_error:
        pop rax
        call _close_fd        ; close memfd_create fd
        jmp _error_exit

_lseek_error:
        pop rax
        call _close_fd        ; close open fd
        pop rax
        call _close_fd        ; close memfd_create fd
        jmp _error_exit

section .bss
        encryption_key   resd 128        
        stub_ending      resq 1

section .rodata
        filename        db 'stub', 0x00        ; name of the stub
        own_path        db '/proc/self', 0x00  ; path to myself to open
