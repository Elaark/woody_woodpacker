#include <elf.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>

#include "data_controller.h"
#include "errors.h"
#include "libft.h"
#include "section_slasher.h"
#include "segment_slasher.h"

static enum exit_code  close_woody(int fd)
// function to delete the new fd created in case a write fails
{
  close(fd);
  if (syscall(87, "woody") == -1)
    return (ERRNO);
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
  total_transfer = 0;
  while (total_transfer < host->loaded_size)
  {
    bytes_written = write(woody, &((char *)host->loaded_file)[total_transfer], host->loaded_size - total_transfer);
    if (bytes_written == -1)
      return (close_woody(woody), ERRNO);
    else
      total_transfer += bytes_written;
  }
  close(woody);
  return (CORRECT);
}
