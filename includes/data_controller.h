#ifndef DATA_CONTROLLER_H
# define DATA_CONTROLLER_H

# include "file_handling.h"


const void  *identify_data_section(const t_file *file, const char *var_name);
const void  *identify_segments(const t_file *file, const char *var_name);

int         search_data_section(const void *section_header, const t_file *file, const char *var_name);
int         search_data_segment(const void *segment_header, const t_file *file, const char *var_name);

const char  *get_string_table(t_file *file);

void        modify_var(t_file *file, const char *var_name);

#endif
