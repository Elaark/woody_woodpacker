section .text
global main
extern _encryption_protocol_

; MAP_ANONYMOUS        equ        1
O_RDONLY        equ        0x00
SEEK_END        equ        0x02

_error_exit:
        mov rax, 0x3c         ; syscall number 60 for exit
        mov rdi, 1            ; error only being 1
        syscall
        ret
        
_close_fd:
        mov rdi, rax
        mov rax, 0x03        ; syscall number 3 for exit
        syscall
        ret                  ; called multipled time

write_intro_message:
        mov rax, 1
        mov rdi, 1
        lea rsi, [intro]
        mov rdx, intro_size
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
        push rax              ; save the current offset in the stack

        mov rax,0x08
        mov rdi, rsp[1]       ; the fd of the current file
        xor rsi, rsi
        mov rdx, SEEK_END     ; off set is set to the size of file + 0
        syscall

        cmp rax, -1
        je _sendfile_error
        push rax
        
        ; putting fd back to where we want
        mov rax, 0x08
        mov rdi, rsp[1] 
        xor rsi, rsi
        syscall

        cmp rax, -1
        je _lseek_other_error
        
        mov rsi, rsp[1]       ; return of lseek
        mov rax, 0x28         ; syscall number 40 for sendfile
        mov rdi, rsp[3]       ; get from stack the new fd, fd_out
        mov rsi, rsp[2]       ; this must be fd in, 
        xor rdx, rdx          ; start from the start of the fd -> set by LSEEK
        mov r10, rsp[0]       ; how much bytes is the host -> from LSEEK to SEEK_END
        syscall

        cmp rax, -1
        je _sendfile_error

        call write_intro_message
        ; execat at the new fd place
       
_normal_exit:
        pop rax
        call _close_fd
        pop rax
        call _close_fd

        mov rax, 0x3c
        xor rdi, rdi
        syscall
        
_memfd_create_error:
        jmp _error_exit

_open_error:
        call _close_fd        ; close memfd_create fd
        jmp _error_exit

_lseek_error:
        call _close_fd        ; close open fd
        jmp _open_error

_lseek_other_error:
        pop rax
        jmp _sendfile_error

_sendfile_error:
        pop rax
        jmp _lseek_error

section .bss
        encryption_key   resd 128        
        stub_ending      resq 1

section .rodata
        filename        db 'stub', 0x00            ; name of the stub
        own_path        db '/proc/self', 0x00      ; path to myself to open
        intro           db '....WOODY.....', 0xA   ; intro message
        intro_size      equ $ - intro
