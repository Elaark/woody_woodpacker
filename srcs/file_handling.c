#include <elf.h>
#include <fcntl.h> // open
#include <stdio.h> // open/close
#include <sys/mman.h> // mmap
#include <unistd.h> // lseek

#include "errors.h"
#include "file_handling.h"

void  open_file(const char *file_name, t_file *data)
{
  data->fd = open(file_name, O_RDWR);
  if (data->fd == -1)
  {
    close_file(data);
    personnal_exit(ERRNO, file_name);
  }
  data->loaded_size = lseek(data->fd, 0, SEEK_END); // lseek can move th offset to the end of the file
  if (data->loaded_size == (off_t)-1)
  {
    close_file(data);
    personnal_exit(ERRNO, file_name);
  }
  lseek(data->fd, 0, SEEK_SET); // Need to make sure to put the offset back to the beginning of the file
  data->loaded_file = mmap(NULL, data->loaded_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, data->fd, 0);
  if (data->loaded_file == MAP_FAILED)
  {
    close_file(data);
    personnal_exit(ERRNO, file_name);
  }
}

void    check_file_eident(t_file *data)
// check if file is correctly a x86 + 64 bits
{
  const char  *arch;
  const char  *endian;

  if (data->loaded_size < EI_NIDENT)
  {
    close_file(data);
    personnal_exit(NOT_A_ELF_FILE, data->filename);
  }
  
  if (*((int *)data->loaded_file) != *(int *)ELFMAG)
  {
    close_file(data);
    personnal_exit(NOT_A_ELF_FILE, data->filename);
  }
  
  arch = &((char *)data->loaded_file)[EI_CLASS];
  if (*arch == ELFCLASSNONE)
  {
    close_file(data);
    personnal_exit(WRONG_ARCH, data->filename);
  }
  data->arch = *arch == ELFCLASS32 ? X32 : X64;

  endian = &((char *)data->loaded_file)[EI_DATA];
  if (*arch == ELFDATANONE)
  {
    close_file(data);
    personnal_exit(WRONG_DATA_ENCODING, data->filename);
  }
  data->endian = *endian == ELFDATA2LSB ? LITTLE : BIG;
}

void  check_file_elf_header_x64(t_file *data)
{
  const Elf64_Ehdr  *header;

  if ((unsigned long)data->loaded_size < sizeof(Elf64_Ehdr))
  {
    close_file(data);
    personnal_exit(FILE_TOO_SHORT, data->filename);
  }

  header = (Elf64_Ehdr *)data->loaded_file; 
  if (header->e_type != ET_EXEC)
  {
    close_file(data);
    personnal_exit(NOT_AN_EXEC_FILE, data->filename);
  }

  if (header->e_machine != EM_386 && header->e_machine != EM_X86_64)
  {
    close_file(data);
    personnal_exit(UNSUPPORTED_CPU_ARCH, data->filename);
  }

  data->sg_table_offset = header->e_phoff;
  data->sg_table_total_entry = header->e_phnum;
  data->sg_table_unit_size = header->e_phentsize;
  if (data->sg_table_offset + data->sg_table_unit_size * data->sg_table_total_entry >= data->loaded_size)
  {
    close_file(data);
    personnal_exit(FILE_TOO_SHORT, data->filename);
  }            // check passed: can access safely program/segment table

}

void  close_file(t_file *data)
{
  if (data->fd != -1)
    close(data->fd);
  if ((data->loaded_file != NULL ||
       data->loaded_file == MAP_FAILED) &&
      data->loaded_size != 0)
    munmap(data->loaded_file, data->loaded_size);
  
}
