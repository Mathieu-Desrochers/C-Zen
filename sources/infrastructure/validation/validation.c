#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/validation/validation.h"

// allocates a validation path
validation_path_t *validation_path_malloc(int property, int index)
{
  validation_path_t *validation_path = malloc(sizeof(validation_path_t));
  check_mem(validation_path);

  validation_path->property = property;
  validation_path->index = index;
  validation_path->next = NULL;

  return validation_path;

error:

  if (validation_path != NULL) { validation_path_free(validation_path); }

  return NULL;
}

// allocates a validation error
validation_error_t *validation_error_malloc(
  int error_code)
{
  validation_error_t *validation_error = NULL;

  validation_error = malloc(sizeof(validation_error_t));
  check_mem(validation_error);

  validation_error->validation_path = NULL;
  validation_error->error_code = error_code;

  return validation_error;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  return NULL;
}

// allocates a validation error
validation_error_t *validation_error_malloc_level_1(
  int property,
  int index,
  int error_code)
{
  validation_error_t *validation_error = NULL;
  validation_path_t *validation_path = NULL;

  validation_error = malloc(sizeof(validation_error_t));
  check_mem(validation_error);

  validation_error->validation_path = NULL;
  validation_error->error_code = error_code;

  validation_path = validation_path_malloc(property, index);
  check_mem(validation_path);

  validation_error->validation_path = validation_path;

  return validation_error;

error:

  if (validation_path != NULL) { validation_path_free(validation_path); }
  if (validation_error != NULL) { validation_error_free(validation_error); }

  return NULL;
}

// allocates a validation error
validation_error_t *validation_error_malloc_level_2(
  int property,
  int index,
  int property_level_2,
  int index_level_2,
  int error_code)
{
  validation_error_t *validation_error = NULL;
  validation_path_t *validation_path = NULL;
  validation_path_t *validation_path_level_2 = NULL;

  validation_error = malloc(sizeof(validation_error_t));
  check_mem(validation_error);

  validation_error->validation_path = NULL;
  validation_error->error_code = error_code;

  validation_path = validation_path_malloc(property, index);
  check_mem(validation_path);

  validation_path_level_2 = validation_path_malloc(property_level_2, index_level_2);
  check_mem(validation_path_level_2);

  validation_error->validation_path = validation_path;
  validation_error->validation_path->next = validation_path_level_2;

  return validation_error;

error:

  if (validation_path_level_2 != NULL) { validation_path_free(validation_path_level_2); }
  if (validation_path != NULL) { validation_path_free(validation_path); }
  if (validation_error != NULL) { validation_error_free(validation_error); }

  return NULL;
}

// adds a validation error to an array
int validation_errors_add(
  validation_error_t ***validation_errors,
  int *allocated_count,
  int *used_count,
  int error_code)
{
  validation_error_t *validation_error = NULL;

  validation_error = validation_error_malloc(error_code);
  check(validation_error != NULL, "validation_error: NULL");

  int array_add_result = array_add_pointer(
    (void ***)validation_errors, allocated_count, used_count,
    (void *)validation_error);

  check(array_add_result == 0, "array_add_result: %d",
    array_add_result);

  validation_error = NULL;

  return 0;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  return -1;
}

// adds a validation error to an array
int validation_errors_add_level_1(
  validation_error_t ***validation_errors,
  int *allocated_count,
  int *used_count,
  int property,
  int index,
  int error_code)
{
  validation_error_t *validation_error = NULL;

  validation_error = validation_error_malloc_level_1(property, index, error_code);
  check(validation_error != NULL, "validation_error: NULL");

  int array_add_result = array_add_pointer(
    (void ***)validation_errors, allocated_count, used_count,
    (void *)validation_error);

  check(array_add_result == 0, "array_add_result: %d",
    array_add_result);

  validation_error = NULL;

  return 0;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  return -1;
}

// adds a validation error to an array
int validation_errors_add_level_2(
  validation_error_t ***validation_errors,
  int *allocated_count,
  int *used_count,
  int property,
  int index,
  int property_level_2,
  int index_level_2,
  int error_code)
{
  validation_error_t *validation_error = NULL;

  validation_error = validation_error_malloc_level_2(property, index, property_level_2, index_level_2, error_code);
  check(validation_error != NULL, "validation_error: NULL");

  int array_add_result = array_add_pointer(
    (void ***)validation_errors, allocated_count, used_count,
    (void *)validation_error);

  check(array_add_result == 0, "array_add_result: %d",
    array_add_result);

  validation_error = NULL;

  return 0;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  return -1;
}

// validates a double
int validate_double(double *value, int required, double min_value, double max_value)
{
  if (value == NULL)
  {
    if (required)
    {
      return VALIDATION_RESULT_REQUIRED;
    }

    return 0;
  }

  if (*value < min_value)
  {
    return VALIDATION_RESULT_TOO_LOW;
  }

  if (*value > max_value)
  {
    return VALIDATION_RESULT_TOO_HIGH;
  }

  return 0;
}

// validates an integer
int validate_int(int *value, int required, int min_value, int max_value)
{
  if (value == NULL)
  {
    if (required)
    {
      return VALIDATION_RESULT_REQUIRED;
    }

    return 0;
  }

  if (*value < min_value)
  {
    return VALIDATION_RESULT_TOO_LOW;
  }

  if (*value > max_value)
  {
    return VALIDATION_RESULT_TOO_HIGH;
  }

  return 0;
}

// validates a string
int validate_string(char *value, int required, int min_length, int max_length)
{
  if (value == NULL)
  {
    if (required)
    {
      return VALIDATION_RESULT_REQUIRED;
    }

    return 0;
  }

  int length = strlen(value);

  if (length < min_length)
  {
    return VALIDATION_RESULT_TOO_SHORT;
  }

  if (length > max_length)
  {
    return VALIDATION_RESULT_TOO_LONG;
  }

  return 0;
}

// validates a time
int validate_time(time_t *value, int required, time_t min_value, time_t max_value)
{
  if (value == NULL)
  {
    if (required)
    {
      return VALIDATION_RESULT_REQUIRED;
    }

    return 0;
  }

  if (*value < min_value)
  {
    return VALIDATION_RESULT_TOO_LOW;
  }

  if (*value > max_value)
  {
    return VALIDATION_RESULT_TOO_HIGH;
  }

  return 0;
}

// validates an array
int validate_array(void **array, int count, int required, int min_count, int max_count)
{
  if (array == NULL)
  {
    if (required)
    {
      return VALIDATION_RESULT_REQUIRED;
    }

    return 0;
  }

  if (count < min_count)
  {
    return VALIDATION_RESULT_TOO_FEW;
  }

  if (count > max_count)
  {
    return VALIDATION_RESULT_TOO_MANY;
  }

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
