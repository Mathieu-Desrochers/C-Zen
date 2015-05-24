#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdlib.h>

// allocates an array of pointers
#define array_malloc(S) ({ void **__a = malloc(sizeof(void *) * (S + 1)); if (__a != NULL) { for (int __i = 0; __i < (S + 1); __i++) { __a[__i] = NULL; } } __a; })

// returns the number of pointers in an array
#define array_count(A) ({ int __i = 0; while (A[__i] != NULL) { __i++; } __i; })

// reallocates an array of pointers
#define array_realloc(A, S) ({ int __c = array_count(A); void **__a = realloc(A, sizeof(void *) * (S + 1)); if (__a != NULL) { for (int __i = __c; __i < (S + 1); __i++) { __a[__i] = NULL; } } __a; })

// frees an array of pointers
#define array_free(A, F) { int __i = 0; while (A[__i] != NULL) { F(A[__i]); __i++; } free(A); }

#endif
