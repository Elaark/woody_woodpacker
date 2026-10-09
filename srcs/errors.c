#include <errno.h>
#include <string.h>

#include "errors.h"
#include "libft.h"

int  code_from_error(enum exit_code e)
{
  if (e == ERRNO)
    return (errno);
  // else if (e == FILE_TOO_SHORT)
  //   return (1);
  // else if (e == UNWRITTABLE)
  //   return (13);
  // else if (e == CORRUPT_ELF_FILE)
  //   return (8);
  else if (e == TOO_MANY_ARGS)
    return (7);
  else
    return (1);
}

char  *msg_from_error(enum exit_code e)
{
  if (e == ERRNO)
    return (strerror(errno));
  else if (e == ASSOCIATED_SEGMENT_MISSING)
    return ("Cannot find associated segment to symbol. Corruption possible");
  else if (e == CORRUPTED_STRTAB_HEADER)
    return ("Index of .strtab header is incoherent and leads ouf of file");
  else if (e == CORRUPTED_STRTAB_OFFSET)
    return ("Offset of .strtab is incoherent and leads ouf of file");
  else if (e == CORRUPTED_SYMBOL_STRTAB_IDX)
    return ("Index inside .strtab is incoherent and leads ouf of file");
  else if (e == DATA_OFFSET_TOO_LARGE)
    return ("Error in segment header value. Corruption possible");
  else if (e == FILE_TOO_SHORT)
    return ("File too short. Headers not completed");
  else if (e == NOT_A_ELF_FILE)
    return ("Not a ELF file. Magic number not found");
  else if (e == NOT_AN_EXEC_FILE)
    return ("ELF file cannot be executed");
  else if (e == TOO_MANY_ARGS)
    return ("Argument list too long");
  else if (e == UNSUPPORTED_CPU_ARCH)
    return ("Arch not supported. Only x86 available"); // meh
  else if (e == KEY_SYMBOL_NOT_FOUND)
    return ("Cannot replace key variable, symbol not found");
  else if (e == WRONG_ARCH || e == WRONG_DATA_ENCODING)
    return ("Error in arch/endian encoding");
  else
    return ("");
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
    return (1);
  sprintf(name, "woody: %s: %s\n", param, msg_from_error(e));
  write(2, name, ft_strlen(name));
  free(name);
  exit(code_from_error(e));
}
