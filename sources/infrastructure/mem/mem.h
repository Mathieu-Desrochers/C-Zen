#ifndef MEM_H
#define MEM_H

#include <time.h>

// allocates and copies a double
int malloc_memcpy_double(double **destination, double *source);

// allocates and copies an integer
int malloc_memcpy_int(int **destination, int *source);

// allocates and copies a string
int malloc_memcpy_string(char **destination, char *source);

// allocates and copies a time
int malloc_memcpy_time(time_t **destination, time_t *source);

#endif
