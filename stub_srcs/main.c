#include <fcntl.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "../includes/aes.h"

unsigned long  stub_ending __attribute__((section(".data"))) = 0;
int            encryption_key[128] __attribute__((section(".data"))) = {0};

int  main(int argc, char *argv[], char *envp[])
{
  uint8_t key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
  };

  uint8_t iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };

  int   exec_host_fd;
  int   self_fd;
  long  host_size;
  long  stub_end_offset;
  long  host_end_offset;

  (void)argc;
  exec_host_fd = syscall(SYS_memfd_create, "stub", 0); // memfd_create
  if (exec_host_fd == -1)
    return (1);

  self_fd = syscall(SYS_openat, AT_FDCWD, "/proc/self/exe", O_RDONLY, 0x00);
  // openat because can work on both x86_64 and arm64
  if (self_fd == -1)
  {
    syscall(SYS_close, exec_host_fd);
    return (1);
  }
  stub_end_offset = syscall(SYS_lseek, self_fd, stub_ending, SEEK_CUR); // lseek
  if (stub_end_offset <= -1)
  {
    syscall(SYS_close, exec_host_fd);
    syscall(SYS_close, self_fd);
    return (1);
  }
  host_end_offset = syscall(SYS_lseek, self_fd, 0, SEEK_END);
  if (host_end_offset <= -1)
  {
    syscall(SYS_close, exec_host_fd);
    syscall(SYS_close, self_fd);
    return (1);
  }
  host_size = host_end_offset - stub_end_offset;
  if (host_size <= 0)
  {
    syscall(SYS_close, exec_host_fd);
    syscall(SYS_close, self_fd);
    return (1);
  }
  stub_end_offset = syscall(SYS_lseek, self_fd, stub_ending, SEEK_SET);
  if (stub_end_offset <= -1)
  {
    syscall(SYS_close, exec_host_fd);
    syscall(SYS_close, self_fd);
    return (1);
  }

  stub_end_offset = 0;
  while (stub_end_offset < host_size)
  {
    host_end_offset = syscall(SYS_sendfile, exec_host_fd, self_fd, NULL, host_size - stub_end_offset); //sendfile
    if (host_end_offset <= 0)
    {
      syscall(SYS_close, exec_host_fd);
      syscall(SYS_close, self_fd);
      return (1);
    }
    stub_end_offset += host_end_offset;
  }
  if (syscall(SYS_close, self_fd) == -1)
  {
    syscall(SYS_close, exec_host_fd);
    return (1);
  }

  lseek(exec_host_fd, 0, SEEK_SET); // Need to make sure to put the offset back to the beginning of the file
  uint8_t *host_data = mmap(NULL, host_size, PROT_READ | PROT_WRITE, MAP_SHARED, exec_host_fd, 0);
  if (host_data == MAP_FAILED)
    return (1);
  
  aes_decrypt_buffer(host_data, host_size, key, iv);
  munmap(host_data, host_size);

  lseek(exec_host_fd, 0, SEEK_SET);
  write(1, "----WOODY-----\n", 15);
  syscall(SYS_execveat, exec_host_fd, "", argv, envp, 0x1000); // execveat with flag AT_EMPTY_PATH

  syscall(SYS_close, exec_host_fd); // shoudl not happen
  return (1);
}
