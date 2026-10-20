#include <stdio.h>

#include "data_controller.h"
#include "errors.h"
#include "stub.h"

enum exit_code  get_the_stub_ready(t_file *stub, const uint8_t key[16], const uint8_t iv[16])
{
  enum exit_code  e;
  off_t           stub_ending;

  stub_ending = (off_t)stub->loaded_size;
  e = replace_data(stub, "stub_ending", &stub_ending, sizeof(stub_ending));
  if (e != CORRECT)
    return (e);
  e = replace_data(stub, "encryption_key", key, 16);
  if (e != CORRECT)
    return (e);
  return (replace_data(stub, "encryption_iv", iv, 16));
}
