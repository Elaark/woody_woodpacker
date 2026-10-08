#ifndef FILE_HANDLING_H
# define FILE_HANDLING_H

// open file
// check ELF + arch
// check for bound ?
// close file

# include <stddef.h>
# include <stdio.h>

typedef enum {
  LITTLE,
  BIG,
} endian_type;

typedef enum {
  X32,
  X64
} arch_type;

typedef struct s_file_data {
  int          fd;
  const char   *filename;
  void         *loaded_file;
  off_t        loaded_size; // important for the stub !
  endian_type  endian;
  arch_type    arch;           // might pack later x32 bits
} t_file;

void    open_file(const char *file_name, t_file *data);
void    check_file(t_file *data);
void    close_file(t_file *data);

#endif
