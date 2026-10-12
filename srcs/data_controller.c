#include <elf.h>

#include "data_controller.h"
#include "errors.h"
#include "libft.h"
#include "section_slasher.h"
#include "segment_slasher.h"

enum exit_code  replace_data(t_file *file, const char *var_name, const char *new_value)
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

  program_header = NULL;
  program_header = identify_segment_associated(file, symbol);
  if (program_header == NULL)
    return (ASSOCIATED_SEGMENT_MISSING);

  data = NULL;
  data = get_mem_emplacement(file, program_header, symbol);
  if (data == NULL)
    return (DATA_OFFSET_TOO_LARGE);
  // printf("size of symbol: %lu\n", symbol->st_size);
  // write(1, new_value, ft_strlen(new_value));
  if (ft_strlen(new_value) != symbol->st_size)
    return (UNFIT_REPLACING_VALUE);
  ft_memcpy(data, new_value, symbol->st_size);
  return (CORRECT);
}
