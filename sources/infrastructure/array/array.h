#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"

// allocates an array of pointers
#define array_malloc(A, C) { A = malloc(sizeof(void *) * (C + 1)); check_mem(A); A[C] = NULL; }

// returns the size of an array of pointers
#define array_size(A) ({ int __i = 0; while (A[__i] != NULL) { __i++; } __i; })

// increases the size of an array of pointers
#define array_realloc(A, C) { A = realloc(A, sizeof(void *) * (C + 1)); check_mem(A); A[C] = NULL; }

// frees an array of pointers
#define array_free(A, F) { int __i = 0; while (A[__i] != NULL) { F(A[__i]); __i++; } free(A); }

#endif
