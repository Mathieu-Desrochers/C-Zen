#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/validation/validation.h"

// allocates a validation path
validation_path_t *validation_path_malloc(int property, int index, validation_path_t *next)
{
  validation_path_t *validation_path = malloc(sizeof(validation_path_t *));
  check_mem(validation_path);

  validation_path->property = property;
  validation_path->index = index;
  validation_path->next = next;

  return validation_path;

error:

  if (validation_path != NULL) { validation_path_free(validation_path); }

  return NULL;
}

// allocates a validation error
validation_error_t *validation_error_malloc(int property, int index, int error_code)
{
  validation_path_t *validation_path = NULL;
  validation_error_t *validation_error = NULL;

  validation_path = validation_path_malloc(property, index, NULL);
  check_mem(validation_path);

  validation_error = malloc(sizeof(validation_error_t *));
  check_mem(validation_error);

  validation_error->validation_path = validation_path;
  validation_error->error_code = error_code;

  return validation_error;

error:

  if (validation_path != NULL) { validation_path_free(validation_path); }
  if (validation_error != NULL) { validation_error_free(validation_error); }

  return NULL;
}

// validates a string
int validate_string(char *value, int required, int min_length, int max_length)
{
  return 0;
}

// frees a validation path
void validation_path_free(validation_path_t *validation_path)
{
  if (validation_path == NULL)
  {
    return;
  }

  validation_path_free(validation_path->next);
  free(validation_path);
}

// frees a validation error
void validation_error_free(validation_error_t *validation_error)
{
  if (validation_error == NULL)
  {
    return;
  }

  validation_path_free(validation_error->validation_path);
  free(validation_error);
}
