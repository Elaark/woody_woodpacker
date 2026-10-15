#include <elf.h>

#include "data_controller.h"
#include "errors.h"
#include "libft.h"
#include "section_slasher.h"
#include "segment_slasher.h"

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
