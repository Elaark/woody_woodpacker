global aes_decrypt_cbc_asm
section .text

; void aes_decrypt_cbc_asm(uint8_t *buffer, size_t length, uint8_t *round_keys, uint8_t *iv);
; ABI System : RDI = buffer, RSI = length, RDX = round_keys, RCX = iv

aes_decrypt_cbc_asm:
    cmp rsi, 0
    jle .end_decrypt

    movdqu xmm11, [rcx]

    movdqu xmm0, [rdx + 0xA0] 

    movdqu xmm1, [rdx + 0x90]
    aesimc xmm1, xmm1

    movdqu xmm2, [rdx + 0x80] 
    aesimc xmm2, xmm2

    movdqu xmm3, [rdx + 0x70]
    aesimc xmm3, xmm3

    movdqu xmm4, [rdx + 0x60]
    aesimc xmm4, xmm4

    movdqu xmm5, [rdx + 0x50] 
    aesimc xmm5, xmm5

    movdqu xmm6, [rdx + 0x40]  
    aesimc xmm6, xmm6

    movdqu xmm7, [rdx + 0x30]      
    aesimc xmm7, xmm7

    movdqu xmm8, [rdx + 0x20]       
    aesimc xmm8, xmm8

    movdqu xmm9, [rdx + 0x10]       
    aesimc xmm9, xmm9

    movdqu xmm10, [rdx + 0x00]  

.decrypt_loop:
    cmp rsi, 0
    jle .end_decrypt

    movdqu xmm12, [rdi]
    movdqa xmm13, xmm12

    pxor xmm12, xmm0

    aesdec xmm12, xmm1 
    aesdec xmm12, xmm2
    aesdec xmm12, xmm3
    aesdec xmm12, xmm4
    aesdec xmm12, xmm5
    aesdec xmm12, xmm6
    aesdec xmm12, xmm7
    aesdec xmm12, xmm8
    aesdec xmm12, xmm9

    aesdeclast xmm12, xmm10 

    pxor xmm12, xmm11 
    
    movdqa xmm11, xmm13

    movdqu [rdi], xmm12

    add rdi, 16
    sub rsi, 16
    jmp .decrypt_loop

.end_decrypt:
    ret