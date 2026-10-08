#ifndef ERRORS_H
# define ERRORS_H

enum exit_code {
  CORRECT,
  TOO_MANY_ARGS,          // used
  ALLOC_ERROR,
  FILE_NOT_FOUND,
  UNWRITTABLE,
  NOT_A_ELF_FILE,         // used
  CORRUPT_ELF_FILE,
  ARCH_NOT_SUPPORTED,
  ERRNO,                  // used
  WRONG_ARCH,             // used
  WRONG_DATA_ENCODING,    // used
  WRONG_CPU_ARCH,         // used
};

int    personnal_exit(enum exit_code e, const char *param);
char  *msg_from_error(enum exit_code e);
int    code_from_error(enum exit_code e);

#endif
