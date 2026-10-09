#include <elf.h>
#include <stdint.h>

#include "endian_swap.h"
#include "section_slasher.h"

void       replace_data(t_file *file)
{
  const void  *symbol;

  symbol = (const void *)identify_symbol(file, TEST_CONTROLLER);
}

Elf64_Sym  *identify_symbol(t_file *file, const char *var_name)
{
  uint16_t  idx;
  uint32_t  sh_type;
  uint64_t  sh_flags;
  Elf64_Shdr  *section_header;

  section_header = file->loaded_file + file->sc_table_offset;
  idx = 0;
  while (idx < file->sc_table_total_entry)
  {
    sh_type = section_header[idx].sh_type;
    if (file->endian == BIG)
      sh_type = SWAP_U32(sh_type);
    sh_flags = section_header[idx].sh_flags;
    if (file->endian == BIG)
      sh_flags = SWAP_U64(sh_flags);
    if (sh_type == SHT_STRTAB && sh_flags == SHF_ALLOC)
      if (compare_symbol(section_header[idx], file->string_table, var_name) == 0)
        return (&section_header[idx]); 
    ++idx;
  }
  return (NULL);
}
