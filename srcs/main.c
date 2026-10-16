#include "bin_compression.h"
#include "data_controller.h"
#include "errors.h"
#include "file_handling.h"
#include "libft.h"
#include "stub.h"

static void  factor_close(t_file *host, t_file *stub, enum exit_code e)
{
    close_file(host);
    close_file(stub);
    personnal_exit(e, host->filename);
}

static int open_and_check(t_file *file)
{
  enum exit_code e;
  
  e = open_file(file->filename, file);
  if (e != CORRECT)
    return (e);
  check_file_eident(file);
  if (file->arch == X32)
    return (WRONG_ARCH);
  return (check_file_elf_header_x64(file));
}

int main(int ac, char **av)
{
  t_file          host; // the binary to be exectuted
  t_file          stub; // the binary that obfuscated the host
  enum exit_code  e;

  if (ac == 1)
    return (personnal_exit(INCORRECT_NB_OF_ARGS, "(no param)"));
  if (ac != 2)
    return (personnal_exit(INCORRECT_NB_OF_ARGS, av[1]));

  ft_bzero(&host, sizeof(t_file));
  ft_bzero(&stub, sizeof(t_file));

  host.filename = av[1];
  stub.filename = "./stub";
  e = open_and_check(&host);
  if (e != CORRECT)
  {
    close_file(&host);
    personnal_exit(e, host.filename);
  }
  e = open_and_check(&stub);
  if (e != CORRECT)
  {
    close_file(&host);
    personnal_exit(e, stub.filename);
  }
  if (host.machine != stub.machine)
    factor_close(&host, &stub, INCOMPATIBLE_ARCH);
  // e = compress_binary(&host);
  // if (e != CORRECT)
  //   factor_close(&host, &stub, ERRNO);

  // add here the encryption protocol, give the key as the NULL pointer here
  
  e = get_the_stub_ready(&stub, NULL);
  if (e != CORRECT)
    factor_close(&host, &stub, e);
  e = join_files(&host, &stub);
  if (e != CORRECT)
    factor_close(&host, &stub, e);
  close_file(&stub);
  close_file(&host); 
}
