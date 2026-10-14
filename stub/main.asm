section .text
global _main
extern _decrypt

; MAP_ANONYMOUS        equ        1

_main:
        mov rax, 0x13f        ; syscall number 319 aka memfd_create
        mov rdi, db           ; filename
        xor rsi, rso          ; Anonymous by default
        syscall               ; memfd_create

        cmp rax, -1
        je _error_exit
        
        ; now I have a fd inside memory (step 1) -> must put my host in it (step 2)
        
_error_exit:
        mov rax, 0x3c
        mov rdi, 1
        syscall

section .bss
        encryption_key   resw 256        
        host_size        resw 1

section .data
        filename        db 'stub', 0x00        ; name of the stub
        
