#ifndef MEM_H
#define MEM_H

#include <time.h>

// allocates and copies a value
int malloc_memcpy_double(double **destination, double *source);
int malloc_memcpy_int(int **destination, int *source);
int malloc_memcpy_string(char **destination, char *source);
int malloc_memcpy_time(time_t **destination, time_t *source);

#endif
