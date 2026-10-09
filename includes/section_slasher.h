#ifndef SECTION_SLASHER_H
# define SECTION_SLASHER_H

# define TEST_CONTROLLER "str"
# include <elf.h>

# include "file_handling.h"

void        replace_data(t_file *file);

const char        *get_string_table(const t_file *file, const Elf64_Shdr *header);
const Elf64_Sym         *identify_symbol_associated(const t_file *file, const char *var_name);
const Elf64_Sym   *compare_symbol(const Elf64_Shdr *section, const t_file *file, const char *var_name);

#endif
