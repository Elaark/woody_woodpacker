#include <elf.h>
#include <stdint.h>

#include "endian_swap.h"
#include "file_handling.h"
#include "segment_slasher.h"

Elf64_Phdr  *identify_segment_associated(const t_file *file, const Elf64_Sym *symbol)
{
  uint64_t    st_value;
  Elf64_Phdr  *segment_header;
  uint32_t    p_type;

  st_value = file->endian == LITTLE ? symbol->st_value : SWAP_U64(symbol->st_value);
  segment_header = file->loaded_file + file->sg_table_offset;
  for (uint16_t idx = 0; idx < file->sg_table_total_entry; ++idx)
  {
    p_type = file->endian == LITTLE ? segment_header->p_type : SWAP_U32(segment_header->p_type);
    if (p_type != PT_LOAD)
      continue ;
    if (symbol_is_outside(file->endian, st_value, &segment_header[idx]) == 0)
      return (&segment_header[idx]);
  }
  return (NULL);
}

int  symbol_is_outside(const endian_type e, uint64_t st_value, const Elf64_Phdr *segment_header)
{
  uint64_t  vaddr;
  uint64_t  filesz;

  vaddr = e == LITTLE ? segment_header->p_vaddr : SWAP_U64(segment_header->p_vaddr);
  filesz = e == LITTLE ? segment_header->p_filesz : SWAP_U64(segment_header->p_filesz);

  if (st_value >= vaddr && st_value < vaddr + filesz)
    return (0);
  else
    return (1);
}

char  *get_mem_emplacement(const t_file *file, const Elf64_Phdr *segment_header, const Elf64_Sym *symbol)
{
  uint64_t  st_value;
  uint64_t  vaddr;
  uint64_t  p_offset;

  st_value = file->endian == LITTLE ? symbol->st_value : SWAP_U64(symbol->st_value);
  vaddr = file->endian == LITTLE ? segment_header->p_vaddr : SWAP_U64(segment_header->p_vaddr);
  p_offset = file->endian == LITTLE ? segment_header->p_offset : SWAP_U64(segment_header->p_offset);

  if ((p_offset + st_value - vaddr) >= (uint64_t)file->loaded_size)
    return (NULL);
  return (file->loaded_file + (p_offset + st_value - vaddr));
  
}
