#ifndef SEGMENT_SLASHER_H
# define SEGMENT_SLASHER_H

# include <elf.h>

# include <file_handling.h>

Elf64_Phdr  *identify_segment_associated(const t_file *file, const Elf64_Sym *symbol);

#endif

