#include <jansson.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/mem/mem.h"

// parses a json string
int json_parse_string(char *string, json_t **json)
{
  json_t *json_return = NULL;

  check(string != NULL, "string: NULL");

  json_return = json_loads(string, JSON_REJECT_DUPLICATES, NULL);
  if (json_return == NULL)
  {
    *json = NULL;
    return -1;
  }

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_parse_string_free(json_return); }

  return -1;
}

// gets a json value from a key
int json_object_get_array(json_t *object, char *key, json_t **value, int *size)
{
  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_array_peek = json_object_get(object, key);
  if (json_array_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_array_peek);
  if (json_typeof_result != JSON_ARRAY)
  {
    *value = NULL;
    return 0;
  }

  *value = json_array_peek;
  *size = json_array_size(json_array_peek);

  return 0;

error:

  return -1;
}

// gets a json value from a key
int json_object_get_bool(json_t *object, char *key, int **value)
{
  int *value_return = NULL;

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_boolean_peek = json_object_get(object, key);
  if (json_boolean_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_boolean_peek);
  if (json_typeof_result != JSON_TRUE && json_typeof_result != JSON_FALSE)
  {
    *value = NULL;
    return 0;
  }

  int value_peek = json_boolean_value(json_boolean_peek);

  int malloc_memcpy_int_result = malloc_memcpy_int(&value_return, &value_peek);
  check(malloc_memcpy_int_result == 0, "malloc_memcpy_int_result: %d",
    malloc_memcpy_int_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// gets a json value from a key
int json_object_get_double(json_t *object, char *key, double **value)
{
  double *value_return = NULL;

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_number_peek = json_object_get(object, key);
  if (json_number_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_number_peek);
  if (json_typeof_result != JSON_INTEGER && json_typeof_result != JSON_REAL)
  {
    *value = NULL;
    return 0;
  }

  double value_peek = json_number_value(json_number_peek);

  int malloc_memcpy_double_result = malloc_memcpy_double(&value_return, &value_peek);
  check(malloc_memcpy_double_result == 0, "malloc_memcpy_double_result: %d",
    malloc_memcpy_double_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// gets a json value from a key
int json_object_get_object(json_t *object, char *key, json_t **value)
{
  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_object_peek = json_object_get(object, key);
  if (json_object_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_object_peek);
  if (json_typeof_result != JSON_OBJECT)
  {
    *value = NULL;
    return 0;
  }

  *value = json_object_peek;

  return 0;

error:

  return -1;
}

// gets a json value from a key
int json_object_get_int(json_t *object, char *key, int **value)
{
  int *value_return = NULL;

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_integer_peek = json_object_get(object, key);
  if (json_integer_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_integer_peek);
  if (json_typeof_result != JSON_INTEGER)
  {
    *value = NULL;
    return 0;
  }

  int value_peek = json_integer_value(json_integer_peek);

  int malloc_memcpy_int_result = malloc_memcpy_int(&value_return, &value_peek);
  check(malloc_memcpy_int_result == 0, "malloc_memcpy_int_result: %d",
    malloc_memcpy_int_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// gets a json value from a key
int json_object_get_string(json_t *object, char *key, char **value)
{
  char *value_return = NULL;

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_string_peek = json_object_get(object, key);
  if (json_string_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_string_peek);
  if (json_typeof_result != JSON_STRING)
  {
    *value = NULL;
    return 0;
  }

  char *value_peek = (char *)json_string_value(json_string_peek);
  if (value_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int malloc_memcpy_string_result = malloc_memcpy_string(&value_return, value_peek);
  check(malloc_memcpy_string_result == 0, "malloc_memcpy_string_result: %d",
    malloc_memcpy_string_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// gets a json value from an index
int json_array_get_array(json_t *array, int index, json_t **value, int *size)
{
  check(array != NULL, "array: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_array_peek = json_array_get(array, index);
  if (json_array_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_array_peek);
  if (json_typeof_result != JSON_ARRAY)
  {
    *value = NULL;
    return 0;
  }

  *value = json_array_peek;
  *size = json_array_size(json_array_peek);

  return 0;

error:

  return -1;
}

// gets a json value from an index
int json_array_get_bool(json_t *array, int index, int **value)
{
  int *value_return = NULL;

  check(array != NULL, "array: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_boolean_peek = json_array_get(array, index);
  if (json_boolean_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_boolean_peek);
  if (json_typeof_result != JSON_TRUE && json_typeof_result != JSON_FALSE)
  {
    *value = NULL;
    return 0;
  }

  int value_peek = json_boolean_value(json_boolean_peek);

  int malloc_memcpy_int_result = malloc_memcpy_int(&value_return, &value_peek);
  check(malloc_memcpy_int_result == 0, "malloc_memcpy_int_result: %d",
    malloc_memcpy_int_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// gets a json value from an index
int json_array_get_double(json_t *array, int index, double **value)
{
  double *value_return = NULL;

  check(array != NULL, "array: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_real_peek = json_array_get(array, index);
  if (json_real_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_real_peek);
  if (json_typeof_result != JSON_INTEGER && json_typeof_result != JSON_REAL)
  {
    *value = NULL;
    return 0;
  }

  double value_peek = json_real_value(json_real_peek);

  int malloc_memcpy_double_result = malloc_memcpy_double(&value_return, &value_peek);
  check(malloc_memcpy_double_result == 0, "malloc_memcpy_double_result: %d",
    malloc_memcpy_double_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// gets a json value from an index
int json_array_get_object(json_t *array, int index, json_t **value)
{
  check(array != NULL, "array: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_object_peek = json_array_get(array, index);
  if (json_object_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_object_peek);
  if (json_typeof_result != JSON_OBJECT)
  {
    *value = NULL;
    return 0;
  }

  *value = json_object_peek;

  return 0;

error:

  return -1;
}

// gets a json value from an index
int json_array_get_int(json_t *array, int index, int **value)
{
  int *value_return = NULL;

  check(array != NULL, "array: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_integer_peek = json_array_get(array, index);
  if (json_integer_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_integer_peek);
  if (json_typeof_result != JSON_INTEGER)
  {
    *value = NULL;
    return 0;
  }

  int value_peek = json_integer_value(json_integer_peek);

  int malloc_memcpy_int_result = malloc_memcpy_int(&value_return, &value_peek);
  check(malloc_memcpy_int_result == 0, "malloc_memcpy_int_result: %d",
    malloc_memcpy_int_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// gets a json value from an index
int json_array_get_string(json_t *array, int index, char **value)
{
  char *value_return = NULL;

  check(array != NULL, "array: NULL");
  check(value != NULL, "value: NULL");

  json_t *json_string_peek = json_array_get(array, index);
  if (json_string_peek == NULL)
  {
    *value = NULL;
    return 0;
  }

  int json_typeof_result = json_typeof(json_string_peek);
  if (json_typeof_result != JSON_STRING)
  {
    *value = NULL;
    return 0;
  }

  char *value_peek = (char *)json_string_value(json_string_peek);

  int malloc_memcpy_string_result = malloc_memcpy_string(&value_return, value_peek);
  check(malloc_memcpy_string_result == 0, "malloc_memcpy_string_result: %d",
    malloc_memcpy_string_result);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// frees a parsed json string
void json_parse_string_free(json_t *json)
{
  if (json == NULL)
  {
    return;
  }

  json_decref(json);
}
