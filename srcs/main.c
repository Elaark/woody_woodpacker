#include "data_controller.h"
#include "errors.h"
#include "file_handling.h"
#include "libft.h"


int  main(int ac, char **av)
{
  t_file  file;
    
  if (ac == 1)
    return (0);
  else if (ac != 4)
    return (personnal_exit(INCORRECT_NB_OF_ARGS, av[1]));

  ft_bzero(&file, sizeof(t_file));
  file.filename = av[1];
  open_file(av[1], &file);
  check_file_eident(&file);
  if (file.arch == X32)                 // will delete that later
  {
    close_file(&file);
    personnal_exit(WRONG_ARCH, av[1]);
  }
  check_file_elf_header_x64(&file);
  replace_data(&file, av[2], av[3]);
  close_file(&file);
}
