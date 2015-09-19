#include <stdlib.h>
#include <string.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/print/print.h"

// prints a string understanding custom conversions
int extended_printf(char* format)
{
  check(format != NULL, "format: NULL");

  char *extended_format = calloc(1024, sizeof(char));

  char *x = format;
  char *y = extended_format;

  while (1)
  {
    char *next_conversion = strstr(x, "%");

    if (next_conversion != NULL)
    {
      int n = next_conversion - x;

      strncpy(y, x, n);
      y = y + n;

      if (strncmp(next_conversion, "%pd", 3) == 0)
      {
        strcpy(y, "%p %d");

        x = next_conversion + 3;
        y = y + 5;
      }
      else if (strncmp(next_conversion, "%ppd", 4) == 0)
      {
        strcpy(y, "%p %p %d");

        x = next_conversion + 4;
        y = y + 8;
      }
      else
      {
        strcpy(y, "%");

        x = next_conversion + 1;
        y = y + 1;
      }

      continue;
    }

    strcpy(y, x);

    break;
  }

  printf("%s\n", extended_format);

  free(extended_format);

  return 0;

error:

  if (extended_format != NULL) { free(extended_format); }

  return -1;
}
