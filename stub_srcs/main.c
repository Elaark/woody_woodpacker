#include <unistd.h>

unsigned long  stub_ending;
int            encryption_key[128];

int  main(int argc, char *argv[], char *envp[])
{
  int   exec_host_fd;
  int   self_fd;
  long  host_size;
  long  stub_end_offset;
  long  host_end_offset;

  (void)argc;
  exec_host_fd = syscall(0x13f, "stub", 0); // memfd_create
  if (exec_host_fd == -1)
    return (1);

  self_fd = syscall(0x02, "/proc/self/exe", 0x00); // 0x00 is for O_RDONLY -> open
  if (self_fd == -1)
  {
    syscall(0x03, exec_host_fd);
    return (1);
  }
  stub_end_offset = syscall(0x08, self_fd, stub_ending, SEEK_CUR); // lseek
  if (stub_end_offset <= -1)
  {
    syscall(0x03, exec_host_fd);
    syscall(0x03, self_fd);
    return (1);
  }
  host_end_offset = syscall(0x08, self_fd, 0, SEEK_END);
  if (host_end_offset <= -1)
  {
    syscall(0x03, exec_host_fd);
    syscall(0x03, self_fd);
    return (1);
  }
  host_size = host_end_offset - stub_end_offset;
  if (host_size <= 0)
  {
    syscall(0x03, exec_host_fd);
    syscall(0x03, self_fd);
    return (1);
  }
  stub_end_offset = syscall(0x08, self_fd, stub_ending, SEEK_SET);
  if (stub_end_offset <= -1)
  {
    syscall(0x03, exec_host_fd);
    syscall(0x03, self_fd);
    return (1);
  }

  stub_end_offset = 0;
  while (stub_end_offset < host_size)
  {
    host_end_offset = syscall(0x28, exec_host_fd, self_fd, NULL, host_size - stub_end_offset); //sendfile
    if (host_end_offset <= 0)
    {
      syscall(0x03, exec_host_fd);
      syscall(0x03, self_fd);
      return (1);
    }
    stub_end_offset += host_end_offset;
  }
  if (syscall(0x03, self_fd) == -1)
  {
    syscall(0x03, exec_host_fd);
    return (1);
  }
  write(1, "----WOODY-----\n", 15);
  syscall(0x142, exec_host_fd, "", argv, envp, 0x1000); // execveat with flag AT_EMPTY_PATH

  syscall(0x03, exec_host_fd); // shoudl not happen
  return (1);
}
