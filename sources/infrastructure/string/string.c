#include <limits.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"

// parses an integer
int string_parse_int(char *string, int *value)
{
  int value_return = 0;

  check_not_null(string);
  check_not_null(value);

  errno = 0;

  char *endptr = NULL;

  value_return = strtol(string, &endptr, 10);

  if ((value_return == LONG_MIN) || (value_return == LONG_MAX))
  {
    if (errno == ERANGE)
    {
      return -1;
    }
  }

  if (*endptr != '\0')
  {
    return -1;
  }

  *value = value_return;

  return 0;

error:

  return -1;
}
