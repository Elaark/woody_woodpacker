#ifndef SECTION_SLASHER_H
# define SECTION_SLASHER_H

# define TEST_CONTROLLER "another_testocourse4"
# include <elf.h>

# include "file_handling.h"

void        replace_data(t_file *file);

const Elf64_Shdr  *get_strtab_header(const t_file *file, const Elf64_Shdr *header);
const char        *get_strtab(const t_file *file, const Elf64_Shdr *strtab_header);
const Elf64_Sym         *identify_symbol_associated(const t_file *file, const char *var_name);
const Elf64_Sym   *compare_symbol(const Elf64_Shdr *section, const t_file *file, const char *var_name);

#endif
