global aes_encrypt_cbc_asm
section .text

; void aes_encrypt_cbc_asm(uint8_t *buffer, size_t length, uint8_t *round_keys, uint8_t *iv);
; ABI System : RDI = buffer, RSI = length, RDX = round_keys, RCX = iv

aes_encrypt_cbc_asm:
    cmp rsi, 0
    jle .done

    movdqu xmm11, [rcx]

    movdqu xmm0, [rdx + 0x00]
    movdqu xmm1, [rdx + 0x10]
    movdqu xmm2, [rdx + 0x20]
    movdqu xmm3, [rdx + 0x30]
    movdqu xmm4, [rdx + 0x40]
    movdqu xmm5, [rdx + 0x50]
    movdqu xmm6, [rdx + 0x60]
    movdqu xmm7, [rdx + 0x70]
    movdqu xmm8, [rdx + 0x80]
    movdqu xmm9, [rdx + 0x90]
    movdqu xmm10, [rdx + 0xA0]

.encrypt_loop:
    cmp rsi, 0
    jle .done

    movdqu xmm12, [rdi]

    pxor xmm12, xmm11 ; CBC: XOR IV
    pxor xmm12, xmm0 ; Initial Round Key (K0)

    aesenc xmm12, xmm1
    aesenc xmm12, xmm2
    aesenc xmm12, xmm3
    aesenc xmm12, xmm4
    aesenc xmm12, xmm5
    aesenc xmm12, xmm6
    aesenc xmm12, xmm7
    aesenc xmm12, xmm8
    aesenc xmm12, xmm9

    aesenclast xmm12, xmm10

    movdqu [rdi], xmm12

    movdqa xmm11, xmm12

    add rdi, 16
    sub rsi, 16
    jmp .encrypt_loop

.done:
    ret