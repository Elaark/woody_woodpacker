#ifndef ERRORS_H
# define ERRORS_H

enum exit_code {
  CORRECT,
  TOO_MANY_ARGS,
  ALLOC_ERROR,
  FILE_NOT_FOUND,
  UNWRITTABLE,
  CORRUPT_ELF_FILE,
  ARCH_NOT_SUPPORTED,
  ERRNO
};

int    personnal_exit(enum exit_code e, const char *param);
char  *msg_from_error(enum exit_code e);
int    code_from_error(enum exit_code e);

#endif
