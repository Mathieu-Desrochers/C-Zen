#include <fcgiapp.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/fastcgi/fastcgi.h"

// reads a stream
int fastcgi_read_stream(FCGX_Stream *stream, char **string)
{
  char *string_return = NULL;

  int exit_code = 0;

  check_not_null(stream);
  check_not_null(string);

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

    char *string_return_temp = NULL;
    string_return_temp = realloc(string_return, buffer_size);
    check_mem(string_return_temp);

    string_return = string_return_temp;
  }

  if (strlen(string_return) == 0)
  {
    free(string_return);

    string_return = NULL;
  }

  *string = string_return;

  goto cleanup;

error:

  if (string_return != NULL) { free(string_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// writes a header
int fastcgi_write_header(FCGX_Stream *stream, char *name, char *value, int is_last)
{
  int exit_code = 0;

  check_not_null(stream);
  check_not_null(name);
  check_not_null(value);

  check_result_not(FCGX_PutS(name, stream), -1);
  check_result_not(FCGX_PutS(": ", stream), -1);
  check_result_not(FCGX_PutS(value, stream), -1);
  check_result_not(FCGX_PutS("\r\n", stream), -1);

  if (is_last == 1)
  {
    check_result_not(FCGX_PutS("\r\n", stream), -1);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// writes a body
int fastcgi_write_body(FCGX_Stream *stream, char *content)
{
  int exit_code = 0;

  check_not_null(stream);
  check_not_null(content);

  check_result_not(FCGX_PutS(content, stream), -1);
  check_result_not(FCGX_PutS("\r\n", stream), -1);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}
