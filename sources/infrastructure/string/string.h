#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <string.h>

#include "../../infrastructure/dbg/dbg.h"

#define string_assign(D, S) { if (D != NULL) { free(D); D = NULL; } if (S != NULL) { D = malloc(strlen(S) + 1); check_mem(D); strcpy(D, S); } }

#endif
