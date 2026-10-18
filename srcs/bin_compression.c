#include <elf.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "bin_compression.h"
#include "errors.h"

bonus_code  stream_byte(const uint8_t *stream)
{
  uint8_t  byte;
  
  
}

bonus_code  compress_binary(t_file *file)
{
  int  new_compressed;

  new_compressed = syscall(SYS_memfd_create, "compressed_file", 0);
  if (new_compressed == -1)
    return (MEMFD_CREATE_FAILURE);
  stream_byte(file->loaded_file);
  // just need to suppress everything exception for PT_LOAD I guess...
  return (0);
}

int  main(int ac, char *av[])
{
  t_file      to_compress;
  bonus_code e;
  
  if (ac != 2)
    return (1);

  if (open_file(av[1], &to_compress) != CORRECT)
    return (2);
  
  e = compress_binary(&to_compress);
  if (e != SUCCESS)
    return (e);
}
