#include "data_controller.h"
#include "errors.h"
#include "file_handling.h"
#include "libft.h"


int  main(int ac, char **av)
{
  t_file  file;
    
  if (ac == 1)
    return (0);
  else if (ac > 2)
    return (personnal_exit(TOO_MANY_ARGS, av[1]));

  ft_bzero(&file, sizeof(t_file));
  open_file(av[1], &file);
  check_file_eident(&file);
  if (file.arch == X32)                 // will delete that later
  {
    close_file(&file);
    personnal_exit(WRONG_ARCH, av[1]);
  }
  check_file_elf_header_x64(&file);
  printf("File is correct: finding data 'str'\n");
  identify_data_section(&file, TEST_CONTROLLER);
  identify_segments(&file, TEST_CONTROLLER);
  // modify_var(&file, TEST_CONTROLLER);
  close_file(&file);
}
