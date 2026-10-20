#ifndef AES_H
#define AES_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define CHUNK_SIZE 4096

int encrypt_buffer_inplace(uint8_t *src, size_t original_size, uint8_t **out_extra_block, size_t *out_extra_len, const uint8_t *key, const uint8_t *iv);
int aes_decrypt_buffer(uint8_t *buf, size_t size, const uint8_t *key, const uint8_t *initial_iv);

 #endif
