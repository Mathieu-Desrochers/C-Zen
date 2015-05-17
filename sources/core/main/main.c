#define _BSD_SOURCE
#define _XOPEN_SOURCE

#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/time/time.h"

int main()
{
  time_t x = 1431872220;

  char *y = format_utc_time(x);
  check(y != NULL, "ooppss");

  printf("%s\n", y);

  free(y);

  return 0;

error:

  if (y != NULL) { free(y); }

  return -1;
}
