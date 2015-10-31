#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"

// allocates and copies a value
int malloc_memcpy_double(double **destination, double *source)
{
  double *destination_return = NULL;

  int exit_code = 0;

  check_not_null(destination);

  if (source == NULL)
  {
    *destination = NULL;
    goto cleanup;
  }

  destination_return = malloc(sizeof(double));
  check_mem(destination_return);

  memcpy(destination_return, source, sizeof(double));

  *destination = destination_return;

  goto cleanup;

error:

  if (destination_return != NULL) { free(destination_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// allocates and copies a value
int malloc_memcpy_int(int **destination, int *source)
{
  int *destination_return = NULL;

  int exit_code = 0;

  check_not_null(destination);

  if (source == NULL)
  {
    *destination = NULL;
    goto cleanup;
  }

  destination_return = malloc(sizeof(int));
  check_mem(destination_return);

  memcpy(destination_return, source, sizeof(int));

  *destination = destination_return;

  goto cleanup;

error:

  if (destination_return != NULL) { free(destination_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// allocates and copies a value
int malloc_memcpy_string(char **destination, char *source)
{
  char *destination_return = NULL;

  int exit_code = 0;

  check_not_null(destination);

  if (source == NULL)
  {
    *destination = NULL;
    goto cleanup;
  }

  destination_return = malloc(strlen(source) + 1);
  check_mem(destination_return);

  strcpy(destination_return, source);

  *destination = destination_return;

  goto cleanup;

error:

  if (destination_return != NULL) { free(destination_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// allocates and copies a value
int malloc_memcpy_time(time_t **destination, time_t *source)
{
  time_t *destination_return = NULL;

  int exit_code = 0;

  check_not_null(destination);

  if (source == NULL)
  {
    *destination = NULL;
    goto cleanup;
  }

  destination_return = malloc(sizeof(time_t));
  check_mem(destination_return);

  memcpy(destination_return, source, sizeof(time_t));

  *destination = destination_return;

  goto cleanup;

error:

  if (destination_return != NULL) { free(destination_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}
