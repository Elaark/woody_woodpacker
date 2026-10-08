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
  check_file(&file);
  printf("File is correct\n");
  close_file(&file);
}
