#include "errors.h"
#include "file_handling.h"
#include "libft.h"


int  main(int ac, char **av)
{
  t_exec  file;
    
  if (ac == 1)
    return (0);
  else if (ac > 2)
    return (personnal_exit(TOO_MANY_ARGS, av[1]));

  ft_bzero(&file, sizeof(t_exec));
  open_file(av[1], &file);
  check_file(&file);
  // do the algo of woody
  close_file(&file)
}
