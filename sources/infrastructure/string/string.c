#include <limits.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"

// parses an integer
int string_parse_int(char *string, int *value)
{
  int value_return = 0;

  int exit_code = 0;

  check_not_null(string);
  check_not_null(value);

  errno = 0;

  char *endptr = NULL;

  value_return = strtol(string, &endptr, 10);

  if ((value_return == (int)LONG_MIN) || (value_return == (int)LONG_MAX))
  {
    if (errno == ERANGE)
    {
      goto error;
    }
  }

  if (*endptr != '\0')
  {
    goto error;
  }

  *value = value_return;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}
