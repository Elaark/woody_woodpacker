#ifndef FILE_HANDLING_H
# define FILE_HANDLING_H

# include <limits.h>
# include <stddef.h>
# include <stdint.h>
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
  off_t        loaded_size;      // important for the stub !
  endian_type  endian;
  arch_type    arch;             // might pack later x32 bits

  off_t        sg_table_offset;  // sg = segment aka program
  uint16_t     sg_table_unit_size;
  uint16_t     sg_table_total_entry;

  off_t        sc_table_offset;
  uint16_t     sc_table_unit_size;
  uint16_t     sc_table_total_entry;
} t_file;

enum exit_code       open_file(const char *file_name, t_file *data);
enum exit_code       check_file_eident(t_file *data);
enum exit_code       check_file_elf_header_x64(t_file *data);
void                 close_file(const t_file *data);

enum exit_code       verify_exec_and_machine(t_file *data);
void                 swap_data_info(t_file *data);

#endif
