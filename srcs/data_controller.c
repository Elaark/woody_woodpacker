#include <elf.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/syscall.h>

#include "aes.h"
#include "data_controller.h"
#include "errors.h"
#include "libft.h"
#include "section_slasher.h"
#include "segment_slasher.h"

static enum exit_code  close_woody(int fd)
// function to delete the new fd created in case a write fails
{
  close(fd);
  if (syscall(SYS_unlinkat, AT_FDCWD, "woody", 0) == -1)
    return (ERRNO);
  // unlinkat over unlink so it can be used by both arm64 and x86_64
  return (WRITE_FAILURE); 
}

enum exit_code  replace_data(t_file *file, const char *var_name, const void *new_value, size_t value_size)
{
  const Elf64_Sym  *symbol;
  enum exit_code   e;
  Elf64_Phdr       *program_header;
  char             *data;

  symbol = NULL;
  e = identify_symbol_associated(file, var_name, &symbol);
  if (e != CORRECT)
    return (e);
  // printf("We found %s symbol\n", var_name);
  if (value_size != symbol->st_size)
    return (UNFIT_REPLACING_VALUE);
  
  program_header = identify_segment_associated(file, symbol);
  if (program_header == NULL)
    return (ASSOCIATED_SEGMENT_MISSING);

  data = get_mem_emplacement(file, program_header, symbol);
  if (data == NULL)
    return (DATA_OFFSET_TOO_LARGE);
  
  ft_memcpy(data, new_value, value_size);
  return (CORRECT);
}

enum exit_code  join_files(t_file *host, t_file *stub)
{
  uint8_t key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
  };

  uint8_t iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };

  int      woody;
  ssize_t  bytes_written;
  long     total_transfer;
  
  woody = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0777);
  if (woody == -1)
    return (ERRNO);
  total_transfer = 0;
  while (total_transfer < stub->loaded_size)
  {
    bytes_written = write(woody, &((char *)stub->loaded_file)[total_transfer], stub->loaded_size - total_transfer);
    if (bytes_written == -1)
      return (close_woody(woody));
    else
      total_transfer += bytes_written;
  }

  uint8_t *extra_block = NULL;
  size_t extra_len = 0;
  size_t crypted_host_mem = 0;

  if (encrypt_buffer_inplace(host->loaded_file, host->loaded_size, &extra_block, &extra_len, key, iv) == 0) {
    crypted_host_mem = host->loaded_size - (host->loaded_size % 16); 
  }

  total_transfer = 0;
  while ((size_t)total_transfer < crypted_host_mem)
  {
    bytes_written = write(woody, &((char *)host->loaded_file)[total_transfer], crypted_host_mem - total_transfer);
    if (bytes_written == -1)
      return (close_woody(woody), ERRNO);
    else
      total_transfer += bytes_written;
  }

  if (extra_block && extra_len > 0) {
    write(woody, &extra_block, extra_len);
    free(extra_block);
  }

  close(woody);
  return (CORRECT);
}
