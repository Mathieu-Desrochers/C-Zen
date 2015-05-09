#ifndef STRING_H
#define STRING_H

#include <string.h>

#define string_duplicate(D, S) { D = malloc(strlen(S) + 1); check_mem(D); strcpy(D, S); }

#endif
