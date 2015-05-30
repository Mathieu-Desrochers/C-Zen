#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../infrastructure/string/string.h"

// duplicates a string
char *strdup(const char *s)
{
  char *d = malloc(strlen(s) + 1);
  if (d == NULL) return NULL;
  strcpy(d, s);
  return d;
}
