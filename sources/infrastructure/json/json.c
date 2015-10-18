#include <jansson.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/mem/mem.h"

// parses a json string
int json_parse_string(char *string, json_t **json)
{
  json_t *json_return = NULL;

  int exit_code = 0;

  check_not_null(string);
  check_not_null(json);

  json_return = json_loads(string, JSON_REJECT_DUPLICATES, NULL);

  *json = json_return;

  goto cleanup;

error:

  if (json_return != NULL) { json_free(json_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from a key
int json_object_get_array(json_t *object, char *key, json_t **value, int *size)
{
  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(value);
  check_not_null(size);

  json_t *json_array_peek = json_object_get(object, key);
  if (json_array_peek == NULL)
  {
    *value = NULL;
    *size = 0;

    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_array_peek);
  if (json_typeof_result != JSON_ARRAY)
  {
    *value = NULL;
    *size = 0;

    goto cleanup;
  }

  *value = json_array_peek;
  *size = json_array_size(json_array_peek);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from a key
int json_object_get_bool(json_t *object, char *key, int **value)
{
  int *value_return = NULL;

  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(value);

  json_t *json_boolean_peek = json_object_get(object, key);
  if (json_boolean_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_boolean_peek);
  if (json_typeof_result != JSON_TRUE && json_typeof_result != JSON_FALSE)
  {
    *value = NULL;
    goto cleanup;
  }

  int value_peek = json_boolean_value(json_boolean_peek);
  check_result(malloc_memcpy_int(&value_return, &value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from a key
int json_object_get_double(json_t *object, char *key, double **value)
{
  double *value_return = NULL;

  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(value);

  json_t *json_number_peek = json_object_get(object, key);
  if (json_number_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_number_peek);
  if (json_typeof_result != JSON_INTEGER && json_typeof_result != JSON_REAL)
  {
    *value = NULL;
    goto cleanup;
  }

  double value_peek = json_number_value(json_number_peek);
  check_result(malloc_memcpy_double(&value_return, &value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from a key
int json_object_get_object(json_t *object, char *key, json_t **value)
{
  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(value);

  json_t *json_object_peek = json_object_get(object, key);
  if (json_object_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_object_peek);
  if (json_typeof_result != JSON_OBJECT)
  {
    *value = NULL;
    goto cleanup;
  }

  *value = json_object_peek;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from a key
int json_object_get_int(json_t *object, char *key, int **value)
{
  int *value_return = NULL;

  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(value);

  json_t *json_integer_peek = json_object_get(object, key);
  if (json_integer_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_integer_peek);
  if (json_typeof_result != JSON_INTEGER)
  {
    *value = NULL;
    goto cleanup;
  }

  int value_peek = json_integer_value(json_integer_peek);
  check_result(malloc_memcpy_int(&value_return, &value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from a key
int json_object_get_string(json_t *object, char *key, char **value)
{
  char *value_return = NULL;

  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(value);

  json_t *json_string_peek = json_object_get(object, key);
  if (json_string_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_string_peek);
  if (json_typeof_result != JSON_STRING)
  {
    *value = NULL;
    goto cleanup;
  }

  char *value_peek = (char *)json_string_value(json_string_peek);
  if (value_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  check_result(malloc_memcpy_string(&value_return, value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from an index
int json_array_get_array(json_t *array, int index, json_t **value, int *size)
{
  int exit_code = 0;

  check_not_null(array);
  check_not_null(value);
  check_not_null(size);

  json_t *json_array_peek = json_array_get(array, index);
  if (json_array_peek == NULL)
  {
    *value = NULL;
    *size = 0;

    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_array_peek);
  if (json_typeof_result != JSON_ARRAY)
  {
    *value = NULL;
    *size = 0;

    goto cleanup;
  }

  *value = json_array_peek;
  *size = json_array_size(json_array_peek);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from an index
int json_array_get_bool(json_t *array, int index, int **value)
{
  int *value_return = NULL;

  int exit_code = 0;

  check_not_null(array);
  check_not_null(value);

  json_t *json_boolean_peek = json_array_get(array, index);
  if (json_boolean_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_boolean_peek);
  if (json_typeof_result != JSON_TRUE && json_typeof_result != JSON_FALSE)
  {
    *value = NULL;
    goto cleanup;
  }

  int value_peek = json_boolean_value(json_boolean_peek);
  check_result(malloc_memcpy_int(&value_return, &value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from an index
int json_array_get_double(json_t *array, int index, double **value)
{
  double *value_return = NULL;

  int exit_code = 0;

  check_not_null(array);
  check_not_null(value);

  json_t *json_real_peek = json_array_get(array, index);
  if (json_real_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_real_peek);
  if (json_typeof_result != JSON_INTEGER && json_typeof_result != JSON_REAL)
  {
    *value = NULL;
    goto cleanup;
  }

  double value_peek = json_real_value(json_real_peek);
  check_result(malloc_memcpy_double(&value_return, &value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from an index
int json_array_get_object(json_t *array, int index, json_t **value)
{
  int exit_code = 0;

  check_not_null(array);
  check_not_null(value);

  json_t *json_object_peek = json_array_get(array, index);
  if (json_object_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_object_peek);
  if (json_typeof_result != JSON_OBJECT)
  {
    *value = NULL;
    goto cleanup;
  }

  *value = json_object_peek;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from an index
int json_array_get_int(json_t *array, int index, int **value)
{
  int *value_return = NULL;

  int exit_code = 0;

  check_not_null(array);
  check_not_null(value);

  json_t *json_integer_peek = json_array_get(array, index);
  if (json_integer_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_integer_peek);
  if (json_typeof_result != JSON_INTEGER)
  {
    *value = NULL;
    goto cleanup;
  }

  int value_peek = json_integer_value(json_integer_peek);
  check_result(malloc_memcpy_int(&value_return, &value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets a json value from an index
int json_array_get_string(json_t *array, int index, char **value)
{
  char *value_return = NULL;

  int exit_code = 0;

  check_not_null(array);
  check_not_null(value);

  json_t *json_string_peek = json_array_get(array, index);
  if (json_string_peek == NULL)
  {
    *value = NULL;
    goto cleanup;
  }

  int json_typeof_result = json_typeof(json_string_peek);
  if (json_typeof_result != JSON_STRING)
  {
    *value = NULL;
    goto cleanup;
  }

  char *value_peek = (char *)json_string_value(json_string_peek);
  check_result(malloc_memcpy_string(&value_return, value_peek), 0);

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// allocates a json context
json_context_t *json_context_malloc()
{
  json_context_t *json_context_return = NULL;

  json_context_return = calloc(1, sizeof(json_context_t));
  check_mem(json_context_return);

  json_context_return->strings = NULL;
  json_context_return->strings_allocated_count = 0;
  json_context_return->strings_used_count = 0;

  goto cleanup;

error:

  if (json_context_return != NULL) { free(json_context_return); }

  json_context_return = NULL;

cleanup:

  return json_context_return;
}

// allocates a json object
json_t *json_object_malloc()
{
  json_t *json_object_return = NULL;

  json_object_return = json_object();
  check_not_null(json_object_return);

  goto cleanup;

error:

  if (json_object_return != NULL) { json_free(json_object_return); }

  json_object_return = NULL;

cleanup:

  return json_object_return;
}

// allocates a json array
json_t *json_array_malloc()
{
  json_t *json_array_return = NULL;

  json_array_return = json_array();
  check_not_null(json_array_return);

  goto cleanup;

error:

  if (json_array_return != NULL) { json_free(json_array_return); }

  json_array_return = NULL;

cleanup:

  return json_array_return;
}

// sets a json value for a key
int json_object_set_array(json_t *object, char *key, json_t *value, json_context_t *json_context)
{
  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(json_context);

  if (value != NULL)
  {
    check_result(json_object_set_new(object, key, value), 0);
  }
  else
  {
    check_result(json_object_set(object, key, json_null()), 0);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// sets a json value for a key
int json_object_set_bool(json_t *object, char *key, int *value, json_context_t *json_context)
{
  json_t *json_boolean_result = NULL;

  int exit_code = 0;

  check_not_null(object);
  check_not_null(key);
  check_not_null(json_context);

  if (value != NULL)
  {
    json_boolean_result = json_boolean(*value);
    check_not_null(json_boolean_result);

    check_result(json_object_set_new(object, key, json_boolean_result), 0);

    json_boolean_result = NULL;
  }
  else
  {
    check_result(json_object_set(object, key, json_null()), 0);
  }

  goto cleanup;

error:

  if (json_boolean_result != NULL) { json_free(json_boolean_result); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// sets a json value for a key
int json_object_set_double(json_t *object, char *key, double *value, json_context_t *json_context)
{
  json_t *json_real_result = NULL;

  check_not_null(object);
  check_not_null(key);
  check_not_null(json_context);

  if (value != NULL)
  {
    json_real_result = json_real(*value);
    check_not_null(json_real_result);

    check_result(json_object_set_new(object, key, json_real_result), 0);

    json_real_result = NULL;
  }
  else
  {
    check_result(json_object_set(object, key, json_null()), 0);
  }

  return 0;

error:

  if (json_real_result != NULL) { json_free(json_real_result); }

  return -1;
}

// sets a json value for a key
int json_object_set_int(json_t *object, char *key, int *value, json_context_t *json_context)
{
  json_t *json_integer_result = NULL;

  check_not_null(object);
  check_not_null(key);
  check_not_null(json_context);

  if (value != NULL)
  {
    json_integer_result = json_integer(*value);
    check_not_null(json_integer_result);

    check_result(json_object_set_new(object, key, json_integer_result), 0);

    json_integer_result = NULL;
  }
  else
  {
    check_result(json_object_set(object, key, json_null()), 0);
  }

  return 0;

error:

  if (json_integer_result != NULL) { json_free(json_integer_result); }

  return -1;
}

// sets a json value for a key
int json_object_set_object(json_t *object, char *key, json_t *value, json_context_t *json_context)
{
  check_not_null(object);
  check_not_null(key);
  check_not_null(json_context);

  if (value != NULL)
  {
    check_result(json_object_set_new(object, key, value), 0);
  }
  else
  {
    check_result(json_object_set(object, key, json_null()), 0);
  }

  return 0;

error:

  return -1;
}

// sets a json value for a key
int json_object_set_string(json_t *object, char *key, char *value, json_context_t *json_context)
{
  json_t *json_string_result = NULL;
  char *value_copied = NULL;

  check_not_null(object);
  check_not_null(key);
  check_not_null(json_context);

  if (value != NULL)
  {
    check_result(malloc_memcpy_string(&value_copied, value), 0);

    json_string_result = json_string(value_copied);
    check_not_null(json_string_result);

    check_result(json_object_set_new(object, key, json_string_result), 0);

    json_string_result = NULL;

    check_result(
      array_add_string(
        &(json_context->strings),
        &(json_context->strings_allocated_count),
        &(json_context->strings_used_count),
        value_copied),
      0);

    value_copied = NULL;
  }
  else
  {
    check_result(json_object_set(object, key, json_null()), 0);
  }

  return 0;

error:

  if (value_copied != NULL) { free(value_copied); }
  if (json_string_result != NULL) { json_free(json_string_result); }

  return -1;
}

// sets a json value for the next index
int json_array_add_array(json_t *array, json_t *value, json_context_t *json_context)
{
  check_not_null(array);
  check_not_null(json_context);

  if (value != NULL)
  {
    check_result(json_array_append_new(array, value), 0);
  }
  else
  {
    check_result(json_array_append(array, json_null()), 0);
  }

  return 0;

error:

  return -1;
}

// sets a json value for the next index
int json_array_add_bool(json_t *array, int *value, json_context_t *json_context)
{
  json_t *json_boolean_result = NULL;

  check_not_null(array);
  check_not_null(json_context);

  if (value != NULL)
  {
    json_boolean_result = json_boolean(*value);
    check_not_null(json_boolean_result);

    check_result(json_array_append_new(array, json_boolean_result), 0);

    json_boolean_result = NULL;
  }
  else
  {
    check_result(json_array_append(array, json_null()), 0);
  }

  return 0;

error:

  if (json_boolean_result != NULL) { json_free(json_boolean_result); }

  return -1;
}

// sets a json value for the next index
int json_array_add_double(json_t *array, double *value, json_context_t *json_context)
{
  json_t *json_real_result = NULL;

  check_not_null(array);
  check_not_null(json_context);

  if (value != NULL)
  {
    json_real_result = json_real(*value);
    check_not_null(json_real_result);

    check_result(json_array_append_new(array, json_real_result), 0);

    json_real_result = NULL;
  }
  else
  {
    check_result(json_array_append(array, json_null()), 0);
  }

  return 0;

error:

  if (json_real_result != NULL) { json_free(json_real_result); }

  return -1;
}

// sets a json value for the next index
int json_array_add_int(json_t *array, int *value, json_context_t *json_context)
{
  json_t *json_integer_result = NULL;

  check_not_null(array);
  check_not_null(json_context);

  if (value != NULL)
  {
    json_integer_result = json_integer(*value);
    check_not_null(json_integer_result);

    check_result(json_array_append_new(array, json_integer_result), 0);

    json_integer_result = NULL;
  }
  else
  {
    check_result(json_array_append(array, json_null()), 0);
  }

  return 0;

error:

  if (json_integer_result != NULL) { json_free(json_integer_result); }

  return -1;
}

// sets a json value for the next index
int json_array_add_object(json_t *array, json_t *value, json_context_t *json_context)
{
  check_not_null(array);
  check_not_null(json_context);

  if (value != NULL)
  {
    check_result(json_array_append_new(array, value), 0);
  }
  else
  {
    check_result(json_array_append(array, json_null()), 0);
  }

  return 0;

error:

  return -1;
}

// sets a json value for the next index
int json_array_add_string(json_t *array, char *value, json_context_t *json_context)
{
  json_t *json_string_result = NULL;
  char *value_copied = NULL;

  check_not_null(array);
  check_not_null(json_context);

  if (value != NULL)
  {
    check_result(malloc_memcpy_string(&value_copied, value), 0);

    json_string_result = json_string(value_copied);
    check_not_null(json_string_result);

    check_result(json_array_append_new(array, json_string_result), 0);

    json_string_result = NULL;

    check_result(
      array_add_string(
        &(json_context->strings),
        &(json_context->strings_allocated_count),
        &(json_context->strings_used_count),
        value_copied),
      0);

    value_copied = NULL;
  }
  else
  {
    check_result(json_array_append(array, json_null()), 0);
  }

  return 0;

error:

  if (json_string_result != NULL) { json_free(json_string_result); }
  if (value_copied != NULL) { free(value_copied); }

  return -1;
}

// formats a json to string
int json_format_string(json_t *json, char **string)
{
  char *json_dumps_result = NULL;

  check_not_null(json);
  check_not_null(string);

  json_dumps_result = json_dumps(json, JSON_INDENT(2) | JSON_PRESERVE_ORDER);
  check_not_null(json_dumps_result);

  *string = json_dumps_result;

  return 0;

error:

  if (json_dumps_result != NULL) { free(json_dumps_result); }

  return -1;
}

// frees a json object
void json_free(json_t *json)
{
  if (json == NULL)
  {
    return;
  }

  json_decref(json);
}

// frees a json context
void json_context_free(json_context_t *json_context)
{
  if (json_context == NULL)
  {
    return;
  }

  array_free_string(json_context->strings, json_context->strings_used_count);

  free(json_context);
}
