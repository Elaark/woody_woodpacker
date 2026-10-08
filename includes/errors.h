#ifndef ERRORS_H
# define ERRORS_H

enum exit_code {
  CORRECT,
  ERRNO,                  // used
  FILE_TOO_SHORT,         // used
  NOT_A_ELF_FILE,         // used
  NOT_AN_EXEC_FILE,       // used
  TOO_MANY_ARGS,          // used
  UNSUPPORTED_CPU_ARCH,         // used
  WRONG_ARCH,             // used
  WRONG_DATA_ENCODING,    // used
};

int    personnal_exit(enum exit_code e, const char *param);
char  *msg_from_error(enum exit_code e);
int    code_from_error(enum exit_code e);

#endif
