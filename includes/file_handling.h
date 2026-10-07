#ifndef FILE_HANDLING_H
# define FILE_HANDLING_H

// open file
// check ELF + arch
// check for bound ?
// close file

# include <stddef.h>
# include <stdio.h>

typedef struct s_exec {
  int         fd;
  const char  *filename;
  void        *loaded_file;
  off_t       loaded_size;
} t_exec;

void    open_file(const char *file_name, t_exec *data);
void    check_file(t_exec *data);
void    close_file(t_exec *data);

#endif
