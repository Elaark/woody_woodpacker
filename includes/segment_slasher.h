#ifndef SEGMENT_SLASHER_H
# define SEGMENT_SLASHER_H

# include <elf.h>

# include <file_handling.h>

Elf64_Phdr  *identify_segment_associated(const t_file *file, const Elf64_Sym *symbol);
int          symbol_is_outside(const endian_type e, uint64_t st_value, const Elf64_Phdr *segment_header);

char  *get_mem_emplacement(const t_file *file, const Elf64_Phdr *segment_header, const Elf64_Sym *symbol);

#endif

