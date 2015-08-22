#include <fcgiapp.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/fastcgi/fastcgi.h"

// reads a stream
int fastcgi_read_stream(FCGX_Stream *stream, char **string)
{
  char *string_return = NULL;
  char *string_return_temp = NULL;

  check(stream != NULL, "stream: NULL");
  check(string != NULL, "string: NULL");

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
      string_return[buffer_offset + get_str_result] = '\0';
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

// writes a header
int fastcgi_write_header(FCGX_Stream *stream, char *name, char *value, int is_last)
{
  int puts_result = 0;

  check(stream != NULL, "stream: NULL");
  check(name != NULL, "name: NULL");
  check(value != NULL, "value: NULL");

  puts_result = FCGX_PutS(name, stream);
  check(puts_result != -1, "puts_result: %d",
    puts_result);

  puts_result = FCGX_PutS(": ", stream);
  check(puts_result != -1, "puts_result: %d",
    puts_result);

  puts_result = FCGX_PutS(value, stream);
  check(puts_result != -1, "puts_result: %d",
    puts_result);

  puts_result = FCGX_PutS("\r\n", stream);
  check(puts_result != -1, "puts_result: %d",
    puts_result);

  if (is_last == 1)
  {
    puts_result = FCGX_PutS("\r\n", stream);
    check(puts_result != -1, "puts_result: %d",
      puts_result);
  }

  return 0;

error:

  return -1;
}
