#include <elf.h>

#include "data_controller.h"
#include "errors.h"
#include "libft.h"
#include "section_slasher.h"
#include "segment_slasher.h"

void  replace_data(t_file *file, const char *var_name, const char *new_value)
{
  const Elf64_Sym  *symbol;
  Elf64_Phdr       *program_header;
  char             *data;

  symbol = identify_symbol_associated(file, var_name);
  if (symbol == NULL)
  {
    close_file(file);
    personnal_exit(KEY_SYMBOL_NOT_FOUND, file->filename);
  }
  // printf("We found %s symbol\n", var_name);
  program_header = identify_segment_associated(file, symbol);
  if (program_header == NULL)
  {
    close_file(file);
    personnal_exit(ASSOCIATED_SEGMENT_MISSING, file->filename);
  }
  data = get_mem_emplacement(file, program_header, symbol);
  if (data == NULL)
  {
    close_file(file);
    personnal_exit(DATA_OFFSET_TOO_LARGE, file->filename);
  }
  // printf("size of symbol: %lu\n", symbol->st_size);
  // write(1, new_value, ft_strlen(new_value));
  if (ft_strlen(new_value) != symbol->st_size)
  {
    close_file(file);
    personnal_exit(UNFIT_REPLACING_VALUE, file->filename);
  }
  ft_memcpy(data, new_value, symbol->st_size);

}
