// #include "data_controller.h"
#include "errors.h"
#include "file_handling.h"
#include "libft.h"
#include "stub.h"


static int open_and_check(t_file *file)
{
  open_file(file->filename, file);
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

  if (ac != 2)
    return (personnal_exit(INCORRECT_NB_OF_ARGS, av[1]));

  ft_bzero(&host, sizeof(t_file));
  ft_bzero(&stub, sizeof(t_file));

  host.filename = av[1];
  stub.filename = "./stub/stub";
  e = open_and_check(&host);
  if (e != CORRECT)
  {
    close_file(&host);
    personnal_exit(e, av[1]);
  }
  // compression_of_binary(&host);
  get_the_stub_ready(&host, &stub);
  close_file(&host);
  
// }


// // main for testing protector hability to clean modify data inside a binary
// int  main(int ac, char **av)
// {
//   t_file          file;
//   enum exit_code  e;
    
//   if (ac == 1)
//     return (0);
//   else if (ac != 4)
//     return (personnal_exit(INCORRECT_NB_OF_ARGS, av[1]));

//   ft_bzero(&file, sizeof(t_file));
//   file.filename = av[1];
//   e = open_and_check(&file);  
//   if (e != CORRECT)
//   {
//     close_file(&file);
//     personnal_exit(e, av[1]);
//   }
//   e = replace_data(&file, av[2], av[3]);
//   if (e != CORRECT)
//   {
//     close_file(&file);
//     personnal_exit(e, av[1]);
//   }
//   close_file(&file);
//   return (0);
// }
