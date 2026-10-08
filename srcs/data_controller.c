#include <elf.h>
#include <stdint.h>

#include "data_controller.h"
#include "errors.h"
#include "libft.h"
#include "file_handling.h"

const void  *identify_data_section(const t_file *file, const char *var_name)
{
  const Elf64_Shdr *section_table;  
  uint16_t         idx;

  section_table = file->loaded_file + file->sc_table_offset;
  idx = 0;
  while (idx < file->sc_table_total_entry)
  {
    if (section_table[idx].sh_flags == (SHF_ALLOC | SHF_WRITE)
          && section_table[idx].sh_type == SHT_PROGBITS)
      if (search_data_section(&section_table[idx], file, var_name) == 0)
          return (&section_table[idx]);
    ++idx;
  }
  return (NULL);
}

const void  *identify_segments(const t_file *file, const char *var_name)
{
  const Elf64_Phdr  *segment_table;
  uint16_t          idx;
  
  segment_table = file->loaded_file + file->sg_table_offset;
  idx = 0;
  while (idx < file->sg_table_total_entry)
  {
    if (segment_table[idx].p_flags == (PF_W | PF_R))
      if (search_data_segment(&segment_table[idx], file, var_name) == 0)
        return (&segment_table[idx]);
    ++idx;
  } // idx should always be good
  // earlier check for bound for sg table entry size/total before
  return (NULL); // No segments holds this data
}

int  search_data_section(const void *section_header, const t_file *file, const char *var_name)
{
  const Elf64_Shdr  *section_hdr;

  section_hdr = (Elf64_Shdr *)section_header;
  if (write(1, file->loaded_file + section_hdr->sh_offset, section_hdr->sh_size) == 0)
    write(2, "\nNo size given\n", ft_strlen("\nNo size given\n"));
  (void)var_name;
  write(1, "\n\n", 2);
  return (1);
}

int  search_data_segment(const void *segment_header, const t_file *file, const char *var_name)
{
  const Elf64_Phdr  *segment_hdr;

  segment_hdr = (Elf64_Phdr *)segment_header;
  if (segment_hdr->p_type == PT_NULL)
    return (1);
  if (write(1, file->loaded_file + segment_hdr->p_offset, segment_hdr->p_filesz) == 0)
    write(2, "\nNo size given\n", ft_strlen("\nNo size given\n"));
  (void)var_name;
  // write(1, "\n\n", 2);
  return (1);
}

// void  modify_var(t_file *file, const char *var_name)
// {
// }
