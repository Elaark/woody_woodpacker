#include <elf.h>
#include <stdint.h>
#include <stdio.h>

#include "endian_swap.h"
#include "errors.h"
#include "file_handling.h"
#include "libft.h"
#include "section_slasher.h"
#include "segment_slasher.h"

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

void       replace_data(t_file *file)
{
  const Elf64_Sym  *symbol;
  Elf64_Phdr       *program_header;
  char             *data;

  symbol = identify_symbol_associated(file, TEST_CONTROLLER);
  if (symbol == NULL)
  {
    close_file(file);
    personnal_exit(KEY_SYMBOL_NOT_FOUND, file->filename);
  }
  printf("We found %s symbol\n", TEST_CONTROLLER);
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
  data[0] = 'J';
}

const Elf64_Sym  *identify_symbol_associated(const t_file *file, const char *var_name)
{
  uint16_t    idx;
  uint32_t    sh_type;
  Elf64_Shdr  *section_header;
  const Elf64_Sym   *symbol;

  section_header = file->loaded_file + file->sc_table_offset;
  idx = 0;
  while (idx < file->sc_table_total_entry)
  {
    sh_type = section_header[idx].sh_type;
    if (file->endian == BIG)
      sh_type = SWAP_U32(sh_type);
    if (sh_type == SHT_SYMTAB)
    {
      symbol = compare_symbol(&section_header[idx], file, var_name);
      if (symbol != NULL)
        return (symbol); 
    }
    ++idx;
  }
  return (NULL);
}

const Elf64_Sym  *compare_symbol(const Elf64_Shdr *section, const t_file *file, const char *var_name)
{
  const Elf64_Shdr  *strtab_header;
  const char        *strtab;
  uint64_t          strtab_size;
  uint16_t          section_size;
  uint64_t          section_offset;
  const Elf64_Sym   *symbol;
  uint32_t          st_name;

  strtab_header = get_strtab_header(file, section);
  if (strtab_header == NULL)
  {
    close_file(file);
    personnal_exit(CORRUPTED_STRTAB_HEADER, file->filename);
  }
  strtab = get_strtab(file, strtab_header);
  if (strtab == NULL)
  {
    close_file(file);
    personnal_exit(CORRUPTED_STRTAB_OFFSET, file->filename);
  }
  strtab_size = file->endian == LITTLE ? strtab_header->sh_size : SWAP_U64(strtab_header->sh_size);
  section_size = file->endian == LITTLE ? section->sh_size : SWAP_U64(section->sh_size);
  section_offset = file->endian == LITTLE ? section->sh_offset : SWAP_U64(section->sh_offset);
  symbol = file->loaded_file + section_offset;

  for (uint16_t idx = 1; idx * sizeof(Elf64_Sym) < section_size; ++idx)
  {
    st_name = file->endian == LITTLE ? symbol[idx].st_name : SWAP_U32(symbol[idx].st_name);
    if (st_name == 0)
      continue ;
    if (st_name >= strtab_size)
    {
      close_file(file);
      personnal_exit(CORRUPTED_SYMBOL_STRTAB_IDX, file->filename);
    }
    if (ft_strncmp(var_name, &strtab[st_name], ft_strlen(var_name) + 1) == 0)
      return (&symbol[idx]);
  }
  return (NULL);
}
