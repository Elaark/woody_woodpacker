#include <errno.h>
#include <sys/random.h>

#include "aes.h"
#include "bin_compression.h"
#include "data_controller.h"
#include "errors.h"
#include "file_handling.h"
#include "libft.h"
#include "stub.h"

static void  factor_close(t_file *host, t_file *stub, enum exit_code e)
{
    close_file(host);
    close_file(stub);
    personnal_exit(e, host->filename);
}

static int open_and_check(t_file *file)
{
  enum exit_code e;
  
  e = open_file(file->filename, file);
  if (e != CORRECT)
    return (e);
  e = check_file_eident(file);
  if (e != CORRECT)
    return (e);
  if (file->arch == X32)
    return (WRONG_ARCH);
  return (check_file_elf_header_x64(file));
}

static int hex_value(char c)
{
  if (c >= '0' && c <= '9')
    return (c - '0');
  if (c >= 'a' && c <= 'f')
    return (c - 'a' + 10);
  if (c >= 'A' && c <= 'F')
    return (c - 'A' + 10);
  return (-1);
}

static enum exit_code parse_key(const char *value, uint8_t key[16])
{
  size_t i;
  int    high;
  int    low;

  if (ft_strlen(value) != 32)
    return (INVALID_KEY);
  i = 0;
  while (i < 16)
  {
    high = hex_value(value[i * 2]);
    low = hex_value(value[i * 2 + 1]);
    if (high == -1 || low == -1)
      return (INVALID_KEY);
    key[i] = (uint8_t)((high << 4) | low);
    i++;
  }
  return (CORRECT);
}

static enum exit_code parse_args(int ac, char **av,
                                const char **filename, uint8_t key[16])
{
  enum exit_code e;

  if (ac >= 2 && ft_strncmp(av[1], "-k", 3) == 0)
  {
    if (ac == 2)
      return (MISSING_KEY);
    e = parse_key(av[2], key);
    if (e != CORRECT)
      return (e);
    if (ac != 4)
      return (INCORRECT_NB_OF_ARGS);
    *filename = av[3];
  }
  else
  {
    if (ac != 2)
      return (INCORRECT_NB_OF_ARGS);
    *filename = av[1];
  }
  return (CORRECT);
}

static enum exit_code random_bytes(uint8_t *buffer, size_t size)
{
  size_t  offset;
  ssize_t count;

  offset = 0;
  while (offset < size)
  {
    count = getrandom(buffer + offset, size - offset, 0);
    if (count == -1)
    {
      if (errno == EINTR)
        continue;
      return (ERRNO);
    }
    if (count == 0)
    {
      errno = EIO;
      return (ERRNO);
    }
    offset += (size_t)count;
  }
  return (CORRECT);
}

int main(int ac, char **av)
{

  t_file          host; // the binary to be exectuted
  t_file          stub; // the binary that obfuscated the host
  enum exit_code  e;
  const char      *filename;
  uint8_t         key[16] = {0};
  uint8_t         iv[16];

  e = parse_args(ac, av, &filename, key);
  if (e != CORRECT)
    return (personnal_exit(e, ac > 1 ? av[1] : "(no param)"));
  
  if (ac == 2)
  {
    e = random_bytes(key, sizeof(key));
    if (e != CORRECT)
      return (personnal_exit(e, "key generation"));
  }
  e = random_bytes(iv, sizeof(iv));
  if (e != CORRECT)
    return (personnal_exit(e, "IV generation"));

  ft_bzero(&host, sizeof(t_file));
  ft_bzero(&stub, sizeof(t_file));

  host.filename = filename;
  stub.filename = "./stub";
  e = open_and_check(&host);
  if (e != CORRECT)
  {
    close_file(&host);
    personnal_exit(e, host.filename);
  }
  e = open_and_check(&stub);
  if (e != CORRECT)
  {
    close_file(&host);
    personnal_exit(e, stub.filename);
  }
  if (host.machine != stub.machine)
    factor_close(&host, &stub, INCOMPATIBLE_ARCH);
  // e = compress_binary(&host);
  // if (e != CORRECT)
  //   factor_close(&host, &stub, ERRNO);

  e = get_the_stub_ready(&stub, key, iv);
  if (e != CORRECT)
    factor_close(&host, &stub, e);
  
  
  e = join_files(&host, &stub, key, iv);
  if (e != CORRECT)
    factor_close(&host, &stub, e);
  close_file(&stub);
  close_file(&host); 
}
