#include <errno.h>
#include <string.h>

#include "errors.h"
#include "libft.h"

int  code_from_error(enum exit_code e)
{
  if (e == ALLOC_ERROR)
    return (12);
  else if (e == TOO_MANY_ARGS)
    return (7);
  else if (e == FILE_NOT_FOUND)
    return (2);
  else if (e == UNWRITTABLE)
    return (13);
  else if (e == CORRUPT_ELF_FILE)
    return (8);
  else if (e == ERRNO)
    return (errno);
  else
    return (1);
}

char  *msg_from_error(enum exit_code e)
{
  if (e == ALLOC_ERROR)
    return ("Cannot allocate memory");
  else if (e == TOO_MANY_ARGS)
    return ("Argument list too long");
  else if (e == FILE_NOT_FOUND)
    return ("No such file or directory");
  else if (e == UNWRITTABLE)
    return ("Permission denied");
  else if (e == CORRUPT_ELF_FILE)
    return ("Exec format error"); // meh
  else if (e == ARCH_NOT_SUPPORTED)
    return ("File architecture not supported. x86_64 only");
  else if (e == ERRNO)
    return (strerror(errno));
  else
    return (0);
}

int  personnal_exit(enum exit_code e, const char *param)
{
  char *name;
  
  if (e == CORRECT)
    exit(0);
  name = ft_calloc(ft_strlen(param) +
                   ft_strlen("woody: : \n") +
                   ft_strlen(msg_from_error(e)) +
                   1, sizeof(char));
  if (name == NULL)
    return (code_from_error(e));
  sprintf(name, "woody: %s: %s\n", param, msg_from_error(e));
  write(2, name, ft_strlen(name));
  free(name);
  exit(code_from_error(e));
}
