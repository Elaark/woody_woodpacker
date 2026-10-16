#ifndef ERRORS_H
# define ERRORS_H

enum exit_code {
  ASSOCIATED_SEGMENT_MISSING,  // used
  CORRECT,
  CORRUPTED_STRTAB_HEADER,     // used
  CORRUPTED_STRTAB_OFFSET,     // used
  CORRUPTED_SYMBOL_STRTAB_IDX, //used
  DATA_OFFSET_TOO_LARGE,
  ERRNO,                       // used
  FILE_TOO_SHORT,              // used
  INCOMPATIBLE_ARCH,           // used
  INCORRECT_NB_OF_ARGS,        // used
  KEY_SYMBOL_NOT_FOUND,        // used
  NOT_A_ELF_FILE,              // used
  NOT_AN_EXEC_FILE,            // used
  PAYLOAD_SIZE_NOT_FOUND,      // NOT USED
  TOO_MANY_ARGS,               // NOT USED
  UNFIT_REPLACING_VALUE,       // used
  UNSUPPORTED_CPU_ARCH,        // used
  WRITE_FAILURE,               // used
  WRONG_ARCH,                  // used
  WRONG_DATA_ENCODING,         // used
};

int    personnal_exit(enum exit_code e, const char *param);
char  *msg_from_error(enum exit_code e);
int    code_from_error(enum exit_code e);

#endif
