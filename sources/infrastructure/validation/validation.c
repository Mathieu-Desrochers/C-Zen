#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/validation/validation.h"

// allocates a validation path
validation_path_t *validation_path_malloc(int property, int index, validation_path_t *next)
{
  validation_path_t *validation_path = malloc(sizeof(validation_path_t));
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
  validation_error_t *validation_error = NULL;
  validation_path_t *validation_path = NULL;

  validation_error = malloc(sizeof(validation_error_t));
  check_mem(validation_error);

  validation_error->validation_path = NULL;
  validation_error->error_code = error_code;

  validation_path = validation_path_malloc(property, index, NULL);
  check_mem(validation_path);

  validation_error->validation_path = validation_path;

  return validation_error;

error:

  if (validation_path != NULL) { validation_path_free(validation_path); }
  if (validation_error != NULL) { validation_error_free(validation_error); }

  return NULL;
}

// adds a validation error to an array
int validation_errors_add(validation_error_t ***validation_errors, int *count, int *used, int property, int index, int error_code)
{
  validation_error_t **reallocated_validation_errors = NULL;
  validation_error_t *validation_error = NULL;

  if (*count == *used)
  {
    int new_count = (*count) == 0 ? 4 : ((*count) * 2);
    reallocated_validation_errors = realloc(*validation_errors, sizeof(validation_error_t *) * new_count);
    check_mem(reallocated_validation_errors);
    *validation_errors = reallocated_validation_errors;
    *count = new_count;
  }

  validation_error = validation_error_malloc(property, index, error_code);
  check(validation_error != NULL, "validation_error: NULL");

  (*validation_errors)[*used] = validation_error;
  (*used)++;

  return 0;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  return -1;
}

// validates a string
int validate_string(char *value, int required, int min_length, int max_length)
{
  return 10;
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

// frees an array of validation errors
void validation_errors_free(validation_error_t **validation_errors, int count)
{
  if (validation_errors == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    validation_error_free(validation_errors[i]);
  }

  free(validation_errors);
}
