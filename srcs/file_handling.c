#include <fcntl.h> // open
#include <stdio.h> // open/close
#include <sys/mman.h> // mmap
#include <unistd.h> // lseek

#include "errors.h"
#include "file_handling.h"

void  open_file(const char *file_name, t_exec *data)
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

void    check_file(t_exec *data)
// check if file is correctly a x86 + 64 bits
{
  
}

void  close_file(t_exec *data)
{
  if (data->fd != -1)
    close(data->fd);
  if ((data->loaded_file != NULL ||
       data->loaded_file == MAP_FAILED) &&
      data->loaded_size != 0)
    munmap(data->loaded_file, data->loaded_size);
  
}
