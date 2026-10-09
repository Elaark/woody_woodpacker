#ifndef SECTION_SLASHER_H
# define SECTION_SLASHER_H

# define TEST_CONTROLLER "str"
# include <elf.h>

# include "file_handling.h"

void        replace_data(t_file *file);
// Elf64_Shdr  *get_symbol_table()c c
Elf64_Sym   *identify_symbol(t_file *file, const char *var_name); // from header_table
Elf64_Sym   *compare_symbol(t_file)

#endif
