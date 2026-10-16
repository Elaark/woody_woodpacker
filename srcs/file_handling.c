#include <elf.h>
#include <fcntl.h> // open
#include <stdint.h>
#include <stdio.h> // open/close
#include <sys/mman.h> // mmap
#include <unistd.h> // lseek

#include "endian_swap.h"
#include "errors.h"
#include "file_handling.h"

enum exit_code  open_file(const char *file_name, t_file *data)
{
  data->fd = open(file_name, O_RDWR);
  if (data->fd == -1)
    return (ERRNO);
  data->loaded_size = lseek(data->fd, 0, SEEK_END); // lseek can move th offset to the end of the file
  if (data->loaded_size == (off_t)-1)
    return (ERRNO);
  lseek(data->fd, 0, SEEK_SET); // Need to make sure to put the offset back to the beginning of the file
  data->loaded_file = mmap(NULL, data->loaded_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, data->fd, 0);
  // no need for MAP_SHARED, I don't modify the original files
  if (data->loaded_file == MAP_FAILED)
    return (ERRNO);
  return (CORRECT);
}

enum exit_code    check_file_eident(t_file *data)
// check if file is correctly a x86 + 64 bits
{
  const char  *arch;
  const char  *endian;

  if (data->loaded_size < EI_NIDENT)
    return (NOT_A_ELF_FILE);
  
  if (*((int *)data->loaded_file) != *(int *)ELFMAG)
    return (NOT_A_ELF_FILE);
  
  arch = &((char *)data->loaded_file)[EI_CLASS];
  if (*arch == ELFCLASSNONE)
    return (WRONG_ARCH);
  data->arch = *arch == ELFCLASS32 ? X32 : X64;

  endian = &((char *)data->loaded_file)[EI_DATA];
  if (*arch == ELFDATANONE)
    return (WRONG_DATA_ENCODING);
  data->endian = *endian == ELFDATA2LSB ? LITTLE : BIG;
  return (CORRECT);
}

enum exit_code  check_file_elf_header_x64(t_file *data)
{
  const Elf64_Ehdr  *header;
  enum exit_code    e;

  if ((unsigned long)data->loaded_size < sizeof(Elf64_Ehdr))
    return (FILE_TOO_SHORT);

  e = verify_exec_and_machine(data);
  if (e != CORRECT)
    return (e);
  header = (Elf64_Ehdr *)data->loaded_file;
  data->sg_table_offset = header->e_phoff;
  data->sg_table_total_entry = header->e_phnum;
  data->sg_table_unit_size = header->e_phentsize;

  data->sc_table_offset = header->e_shoff;
  data->sc_table_unit_size = header->e_shentsize;
  data->sc_table_total_entry = header->e_shnum;

  if (data->endian == BIG)
    swap_data_info(data);
  
  if (data->sg_table_offset + data->sg_table_unit_size * data->sg_table_total_entry >= data->loaded_size)
    return (FILE_TOO_SHORT); // check passed: can access safely program/segment table
  if (data->sc_table_offset + data->sc_table_unit_size * data->sg_table_total_entry >= data->loaded_size)
    return (FILE_TOO_SHORT);
  return (CORRECT);
}

void  close_file(const t_file *data)
{
  if ((data->loaded_file != NULL ||
       data->loaded_file == MAP_FAILED) &&
      data->loaded_size != 0)
    munmap(data->loaded_file, data->loaded_size);  
  if (data->fd != -1)
    close(data->fd);
}

enum exit_code  verify_exec_and_machine(t_file *data)
{
  Elf64_Ehdr *header;
  uint16_t   e_type;
  
  header = (Elf64_Ehdr *)data->loaded_file;
  e_type = header->e_type;
  data->machine = header->e_machine;
  if (data->endian == BIG)
  {
    e_type = SWAP_U16(e_type);
    data->machine = SWAP_U16(data->machine);
  }
  if (e_type != ET_EXEC)
    return (NOT_AN_EXEC_FILE);
  if (data->machine != EM_386 && data->machine != EM_X86_64 && data->machine != EM_AARCH64)
    return (UNSUPPORTED_CPU_ARCH);
  return (CORRECT);
}

// if we have an big-endian file
void  swap_data_info(t_file *data)
{
  data->sg_table_offset = SWAP_U64(data->sg_table_offset);
  data->sg_table_unit_size = SWAP_U16(data->sg_table_unit_size);
  data->sg_table_total_entry = SWAP_U16(data->sg_table_total_entry);

  data->sc_table_offset = SWAP_U64(data->sc_table_offset);
  data->sc_table_unit_size = SWAP_U16(data->sc_table_unit_size);
  data->sc_table_total_entry = SWAP_U16(data->sc_table_total_entry);
}
