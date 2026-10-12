#include <elf.h>
#include <stdint.h>
#include <stdio.h>

#include "endian_swap.h"
#include "errors.h"
#include "file_handling.h"
#include "libft.h"
#include "section_slasher.h"

const Elf64_Shdr  *get_strtab_header(const t_file *file, const Elf64_Shdr *header)
{
  const Elf64_Shdr  *section_table;
  uint32_t          sh_link;

  section_table = file->loaded_file + file->sc_table_offset;
  sh_link = file->endian == LITTLE ? header->sh_link : SWAP_U32(header->sh_link);

  if (sh_link >= file->sc_table_total_entry)
    return (NULL);
  return (&section_table[sh_link]);
 }

const char  *get_strtab(const t_file *file, const Elf64_Shdr *strtab_header)
{ 
  off_t             sh_offset;
  
  sh_offset = strtab_header->sh_offset;
  if (file->endian == BIG)
    sh_offset = SWAP_U64(sh_offset);

  if (sh_offset >= file->loaded_size)
    return (NULL);
  
  return (file->loaded_file + sh_offset);
}

enum exit_code  identify_symbol_associated(const t_file *file, const char *var_name, const Elf64_Sym **symbol)
{
  enum exit_code    e;
  uint16_t          idx;
  uint32_t          sh_type;
  Elf64_Shdr        *section_header;

  section_header = file->loaded_file + file->sc_table_offset;
  idx = 0;
  while (idx < file->sc_table_total_entry)
  {
    sh_type = section_header[idx].sh_type;
    if (file->endian == BIG)
      sh_type = SWAP_U32(sh_type);
    if (sh_type == SHT_SYMTAB)
    {
      e = compare_symbol(&section_header[idx], file, var_name, symbol);
      if (e != CORRECT)
        return (e); 
      if (*symbol != NULL)
        return (CORRECT);
    }
    ++idx;
  }
  return (KEY_SYMBOL_NOT_FOUND);
}

enum exit_code  compare_symbol(const Elf64_Shdr *section, const t_file *file, const char *var_name, const Elf64_Sym **symbol)
{
  const Elf64_Shdr  *strtab_header;
  const char        *strtab;
  uint64_t          strtab_size;
  uint16_t          section_size;
  uint64_t          section_offset;
  uint32_t          st_name;

  strtab_header = get_strtab_header(file, section);
  if (strtab_header == NULL)
    return (CORRUPTED_STRTAB_HEADER);
  strtab = get_strtab(file, strtab_header);
  if (strtab == NULL)
    return (CORRUPTED_STRTAB_OFFSET);
  strtab_size = file->endian == LITTLE ? strtab_header->sh_size : SWAP_U64(strtab_header->sh_size);
  section_size = file->endian == LITTLE ? section->sh_size : SWAP_U64(section->sh_size);
  section_offset = file->endian == LITTLE ? section->sh_offset : SWAP_U64(section->sh_offset);

  *symbol = file->loaded_file + section_offset;

  for (uint16_t idx = 1; idx * sizeof(Elf64_Sym) < section_size; ++idx)
  {
    st_name = file->endian == LITTLE ? (*symbol)[idx].st_name : SWAP_U32((*symbol[idx]).st_name);
    if (st_name == 0)
      continue ;
    if (st_name >= strtab_size)
      return (CORRUPTED_SYMBOL_STRTAB_IDX);
    if (ft_strncmp(var_name, &strtab[st_name], ft_strlen(var_name) + 1) == 0)
    {
      *symbol = &((*symbol)[idx]);
      return (CORRECT);
    }
  }
  *symbol = NULL;
  return (CORRECT);
}
