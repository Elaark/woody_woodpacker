default rel

O_RDONLY        equ 0
SEEK_SET        equ 0
SEEK_END        equ 2
AT_EMPTY_PATH   equ 0x1000
EINTR           equ 4

section .text
global main

; Called by the C runtime: edi = argc, rsi = argv, rdx = envp.
main:
        push rbp
        push rbx
        push r12
        push r13
        push r14
        push r15

        mov r14, rsi                ; Preserve argv across syscalls.
        mov r15, rdx                ; Preserve envp across syscalls.
        mov r12, -1                 ; memfd, when open
        mov r13, -1                 ; self fd, when open

        mov eax, 319                ; memfd_create
        lea rdi, [filename]
        xor esi, esi
        syscall
        test rax, rax
        js .error
        mov r12, rax

        mov eax, 2                  ; open
        lea rdi, [own_path]
        mov esi, O_RDONLY
        xor edx, edx
        syscall
        test rax, rax
        js .error
        mov r13, rax

        mov rbp, [stub_ending]       ; File offset, not the symbol's address.
        test rbp, rbp
        js .error
        mov eax, 8                  ; lseek: find the complete file size
        mov rdi, r13
        xor esi, esi
        mov edx, SEEK_END
        syscall
        test rax, rax
        js .error
        sub rax, rbp
        jle .error                  ; The appended host must not be empty.
        mov rbx, rax                ; Bytes remaining to copy

        mov eax, 8                  ; lseek: return to the host's first byte
        mov rdi, r13
        mov rsi, rbp
        mov edx, SEEK_SET
        syscall
        test rax, rax
        js .error

.copy_host:
        mov eax, 40                 ; sendfile
        mov rdi, r12
        mov rsi, r13
        xor edx, edx                ; Use and advance the input fd's offset.
        mov r10, rbx
        syscall
        cmp rax, -EINTR
        je .copy_host
        test rax, rax
        jle .error                  ; Error or EOF before the host is complete
        sub rbx, rax
        jnz .copy_host

        mov eax, 3                  ; close self fd exactly once
        mov rdi, r13
        syscall
        mov r13, -1
        test rax, rax
        js .error

        ; Decryption can be added here when the host is encrypted.
        lea rbp, [intro]
        mov ebx, intro_size
.write_intro:
        mov eax, 1                  ; write
        mov edi, 1
        mov rsi, rbp
        mov rdx, rbx
        syscall
        cmp rax, -EINTR
        je .write_intro
        test rax, rax
        jle .error
        add rbp, rax
        sub rbx, rax
        jnz .write_intro

        mov eax, 322                ; execveat(memfd, "", argv, envp, AT_EMPTY_PATH)
        mov rdi, r12
        lea rsi, [empty_path]
        mov rdx, r14
        mov r10, r15
        mov r8d, AT_EMPTY_PATH
        syscall                     ; Returns only on failure.

.error:
        test r13, r13
        js .close_memfd
        mov eax, 3
        mov rdi, r13
        syscall
.close_memfd:
        test r12, r12
        js .return_error
        mov eax, 3
        mov rdi, r12
        syscall
.return_error:
        mov eax, 1
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbx
        pop rbp
        ret

section .data
; Keep symbol sizes available to replace_data() in the packer.
global encryption_key:data (512)
global stub_ending:data (8)
        encryption_key  times 128 dd 0
        stub_ending     dq 0

section .rodata
        filename        db 'stub', 0
        own_path        db '/proc/self/exe', 0
        empty_path      db 0
        intro           db '----WOODY-----', 10
        intro_size      equ $ - intro

section .note.GNU-stack noalloc noexec nowrite progbits
