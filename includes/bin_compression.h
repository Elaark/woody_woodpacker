#ifndef BIN_COMPRESSION_H
# define BIN_COMPRESSION_H

# include "file_handling.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef enum bonus_return_code {
  SUCCESS,
  MEMFD_CREATE_FAILURE,
} bonus_code;

typedef struct compressed_file {
  off_t     offset;
  size_t    compressed_size;
  size_t    original_size;
  uint32_t  check_sum;  
} c_file;  // add this struct to executable stub so I can use it as metadata for decrompretion algorithm

bonus_code  compress_binary(t_file *file);

#endif
