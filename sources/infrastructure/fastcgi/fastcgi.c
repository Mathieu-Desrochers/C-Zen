#include <fcgiapp.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/fastcgi/fastcgi.h"

// reads a stream
int fastcgi_read_stream(FCGX_Stream *stream, char **string)
{
  char *string_return = NULL;
  char *string_return_temp = NULL;

  int buffer_size = 32;
  int buffer_offset = 0;

  string_return = malloc(sizeof(char) * buffer_size);
  check_mem(string_return);

  while (1)
  {
    char *location = string_return + buffer_offset;
    int length = buffer_size - buffer_offset;

    int get_str_result = FCGX_GetStr(location, length, stream);

    if (get_str_result < length)
    {
      string_return[buffer_offset + get_str_result] = NULL;
      break;
    }

    buffer_offset = buffer_size;
    buffer_size = buffer_size * 2;

    string_return_temp = realloc(string_return, buffer_size);
    check_mem(string_return_temp);

    string_return = string_return_temp;
    string_return_temp = NULL;
  }

  *string = string_return;

  return 0;

error:

  if (string_return != NULL) { free(string_return); }

  return -1;
}
