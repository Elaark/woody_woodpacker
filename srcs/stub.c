#include <stdio.h>

#include "data_controller.h"
#include "errors.h"
#include "stub.h"

enum exit_code  get_the_stub_ready(t_file *stub, int *encryption_key)
{
  enum exit_code  e;
  off_t           stub_ending;

  stub_ending = (off_t)stub->loaded_size;
  (void)encryption_key;
  e = replace_data(stub, "stub_ending", &stub_ending, sizeof(stub_ending));
  return (e);
}
