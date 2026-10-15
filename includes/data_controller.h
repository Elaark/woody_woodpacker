#ifndef DATA_CONTROLLER_H
# define DATA_CONTROLLER_H

# include "file_handling.h"

enum exit_code  replace_data(t_file *file, const char *var_name, const void *new_value, size_t value_size);

#endif

