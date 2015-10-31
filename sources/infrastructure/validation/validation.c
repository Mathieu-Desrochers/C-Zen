#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/validation/validation.h"

// allocates a validation path
validation_path_t *validation_path_malloc(int property, int index)
{
  validation_path_t *validation_path_return = NULL;

  validation_path_return = calloc(1, sizeof(validation_path_t));
  check_mem(validation_path_return);

  validation_path_return->property = property;
  validation_path_return->index = index;
  validation_path_return->next = NULL;

  goto cleanup;

error:

  if (validation_path_return != NULL) { validation_path_free(validation_path_return); }

  validation_path_return = NULL;

cleanup:

  return validation_path_return;
}

// allocates a validation error
validation_error_t *validation_error_malloc(
  int error_code)
{
  validation_error_t *validation_error_return = NULL;

  validation_error_return = calloc(1, sizeof(validation_error_t));
  check_mem(validation_error_return);

  validation_error_return->validation_path = NULL;
  validation_error_return->error_code = error_code;

  goto cleanup;

error:

  if (validation_error_return != NULL) { validation_error_free(validation_error_return); }

  validation_error_return = NULL;

cleanup:

  return validation_error_return;
}

// allocates a validation error
validation_error_t *validation_error_malloc_level_1(
  int property,
  int index,
  int error_code)
{
  validation_error_t *validation_error_return = NULL;
  validation_path_t *validation_path_return = NULL;

  validation_error_return = calloc(1, sizeof(validation_error_t));
  check_mem(validation_error_return);

  validation_path_return = validation_path_malloc(property, index);
  check_not_null(validation_path_return);

  validation_error_return->validation_path = validation_path_return;
  validation_error_return->error_code = error_code;

  goto cleanup;

error:

  if (validation_error_return != NULL) { validation_error_free(validation_error_return); }
  if (validation_path_return != NULL) { validation_path_free(validation_path_return); }

  validation_error_return = NULL;
  validation_path_return = NULL;

cleanup:

  return validation_error_return;
}

// allocates a validation error
validation_error_t *validation_error_malloc_level_2(
  int property,
  int index,
  int property_level_2,
  int index_level_2,
  int error_code)
{
  validation_error_t *validation_error_return = NULL;
  validation_path_t *validation_path_return = NULL;
  validation_path_t *validation_path_level_2_return = NULL;

  validation_error_return = calloc(1, sizeof(validation_error_t));
  check_mem(validation_error_return);

  validation_path_return = validation_path_malloc(property, index);
  check_not_null(validation_path_return);

  validation_path_level_2_return = validation_path_malloc(property_level_2, index_level_2);
  check_not_null(validation_path_level_2_return);

  validation_error_return->validation_path = validation_path_return;
  validation_error_return->validation_path->next = validation_path_level_2_return;
  validation_error_return->error_code = error_code;

  goto cleanup;

error:

  if (validation_error_return != NULL) { validation_error_free(validation_error_return); }
  if (validation_path_return != NULL) { validation_path_free(validation_path_return); }
  if (validation_path_level_2_return != NULL) { validation_path_free(validation_path_level_2_return); }

  validation_error_return = NULL;
  validation_path_return = NULL;
  validation_path_level_2_return = NULL;

cleanup:

  return validation_error_return;
}

// adds a validation error to an array
int validation_errors_add(
  validation_error_t ***validation_errors,
  int *allocated_count,
  int *used_count,
  int error_code)
{
  int exit_code = 0;

  check_not_null(validation_errors);
  check_not_null(allocated_count);
  check_not_null(used_count);

  validation_error_t *validation_error = NULL;

  validation_error = validation_error_malloc(error_code);
  check_not_null(validation_error);

  check_result(
    array_add_pointer(
      (void ***)validation_errors,
      allocated_count,
      used_count,
      (void *)validation_error),
    0);

  validation_error = NULL;

  goto cleanup;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  exit_code = -1;

cleanup:

  return exit_code;
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
  int exit_code = 0;

  validation_error_t *validation_error = NULL;

  check_not_null(validation_errors);
  check_not_null(allocated_count);
  check_not_null(used_count);

  validation_error = validation_error_malloc_level_1(property, index, error_code);
  check_not_null(validation_error);

  check_result(
    array_add_pointer(
      (void ***)validation_errors,
      allocated_count,
      used_count,
      (void *)validation_error),
    0);

  validation_error = NULL;

  goto cleanup;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  exit_code = -1;

cleanup:

  return exit_code;
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
  int exit_code = 0;

  validation_error_t *validation_error = NULL;

  check_not_null(validation_errors);
  check_not_null(allocated_count);
  check_not_null(used_count);

  validation_error = validation_error_malloc_level_2(property, index, property_level_2, index_level_2, error_code);
  check_not_null(validation_error);

  check_result(
    array_add_pointer(
      (void ***)validation_errors,
      allocated_count,
      used_count,
      (void *)validation_error),
    0);

  validation_error = NULL;

  goto cleanup;

error:

  if (validation_error != NULL) { validation_error_free(validation_error); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// validates a value
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

// validates a value
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

// validates a value
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

// validates a value
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
