#include <jansson.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/mem/mem.h"

// parses a json string
int json_parse_string(char *string, json_t **json)
{
  json_t *json_return = NULL;

  check(string != NULL, "string: NULL");
  check(json != NULL, "json: NULL");

  json_return = json_loads(string, JSON_REJECT_DUPLICATES, NULL);
  if (json_return == NULL)
  {
    *json = NULL;

    return 0;
  }

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}

// gets a json value from a key
int json_object_get_array(json_t *object, char *key, json_t **value, int *size)
{
  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(value != NULL, "value: NULL");
  check(size != NULL, "size: NULL");

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
  check(size != NULL, "size: NULL");

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

// allocates a json context
int json_context_malloc(json_context_t **json_context)
{
  json_context_t *json_context_return = NULL;

  check(json_context != NULL, "json_context: NULL");

  json_context_return = malloc(sizeof(json_context_t));
  check_mem(json_context_return);

  json_context_return->strings = NULL;
  json_context_return->strings_allocated_count = 0;
  json_context_return->strings_used_count = 0;

  *json_context = json_context_return;

  return 0;

error:

  if (json_context_return != NULL) { free(json_context_return); }

  return -1;
}

// allocates a json object
int json_object_malloc(json_t **json)
{
  json_t *json_return = NULL;

  check(json != NULL, "json: NULL");

  json_return = json_object();
  check(json_return != NULL, "json_return: NULL");

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}

// allocates a json array
int json_array_malloc(json_t **json)
{
  json_t *json_result = NULL;

  check(json != NULL, "json: NULL");

  json_result = json_array();
  check(json_result != NULL, "json_result: NULL");

  *json = json_result;

  return 0;

error:

  if (json_result != NULL) { json_free(json_result); }

  return -1;
}

// sets a json value for a key
int json_object_set_array(json_t *object, char *key, json_t *value, json_context_t *json_context)
{
  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    int json_object_set_result = json_object_set(object, key, value);
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
  }
  else
  {
    int json_object_set_result = json_object_set(object, key, json_null());
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
  }

  return 0;

error:

  return -1;
}

// sets a json value for a key
int json_object_set_bool(json_t *object, char *key, int *value, json_context_t *json_context)
{
  json_t *json_boolean_result = NULL;

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    json_boolean_result = json_boolean(*value);
    check(json_boolean_result != NULL, "json_boolean_result: NULL");

    int json_object_set_result = json_object_set_new(object, key, json_boolean_result);
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);

    json_boolean_result = NULL;
  }
  else
  {
    int json_object_set_result = json_object_set(object, key, json_null());
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
  }

  return 0;

error:

  if (json_boolean_result != NULL) { json_free(json_boolean_result); }

  return -1;
}

// sets a json value for a key
int json_object_set_double(json_t *object, char *key, double *value, json_context_t *json_context)
{
  json_t *json_real_result = NULL;

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    json_real_result = json_real(*value);
    check(json_real_result != NULL, "json_real_result: NULL");

    int json_object_set_result = json_object_set_new(object, key, json_real_result);
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);

    json_real_result = NULL;
  }
  else
  {
    int json_object_set_result = json_object_set(object, key, json_null());
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
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

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    json_integer_result = json_integer(*value);
    check(json_integer_result != NULL, "json_integer_result: NULL");

    int json_object_set_result = json_object_set_new(object, key, json_integer_result);
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);

    json_integer_result = NULL;
  }
  else
  {
    int json_object_set_result = json_object_set(object, key, json_null());
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
  }

  return 0;

error:

  if (json_integer_result != NULL) { json_free(json_integer_result); }

  return -1;
}

// sets a json value for a key
int json_object_set_object(json_t *object, char *key, json_t *value, json_context_t *json_context)
{
  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    int json_object_set_result = json_object_set(object, key, value);
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
  }
  else
  {
    int json_object_set_result = json_object_set(object, key, json_null());
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
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

  check(object != NULL, "object: NULL");
  check(key != NULL, "key: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    int malloc_memcpy_string_result = malloc_memcpy_string(&value_copied, value);
    check(malloc_memcpy_string_result == 0, "malloc_memcpy_string_result: %d",
      malloc_memcpy_string_result);

    json_string_result = json_string(value_copied);
    check(json_string_result != NULL, "json_string_result: NULL");

    int json_object_set_result = json_object_set_new(object, key, json_string_result);
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);

    json_string_result = NULL;

    int array_add_result = array_add_string(
      &(json_context->strings),
      &(json_context->strings_allocated_count),
      &(json_context->strings_used_count),
      value_copied);

    check(array_add_result == 0, "array_add_result: %d",
      array_add_result);

    value_copied = NULL;
  }
  else
  {
    int json_object_set_result = json_object_set(object, key, json_null());
    check(json_object_set_result == 0, "json_object_set_result: %d",
      json_object_set_result);
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
  check(array != NULL, "array: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    int json_array_append_result = json_array_append(array, value);
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
  }
  else
  {
    int json_array_append_result = json_array_append(array, json_null());
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
  }

  return 0;

error:

  return -1;
}

// sets a json value for the next index
int json_array_add_bool(json_t *array, int *value, json_context_t *json_context)
{
  json_t *json_boolean_result = NULL;

  check(array != NULL, "array: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    json_boolean_result = json_boolean(*value);
    check(json_boolean_result != NULL, "json_boolean_result: NULL");

    int json_array_append_result = json_array_append_new(array, json_boolean_result);
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);

    json_boolean_result = NULL;
  }
  else
  {
    int json_array_append_result = json_array_append(array, json_null());
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
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

  check(array != NULL, "array: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    json_real_result = json_real(*value);
    check(json_real_result != NULL, "json_real_result: NULL");

    int json_array_append_result = json_array_append_new(array, json_real_result);
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);

    json_real_result = NULL;
  }
  else
  {
    int json_array_append_result = json_array_append(array, json_null());
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
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

  check(array != NULL, "array: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    json_integer_result = json_integer(*value);
    check(json_integer_result != NULL, "json_integer_result: NULL");

    int json_array_append_result = json_array_append_new(array, json_integer_result);
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);

    json_integer_result = NULL;
  }
  else
  {
    int json_array_append_result = json_array_append(array, json_null());
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
  }

  return 0;

error:

  if (json_integer_result != NULL) { json_free(json_integer_result); }

  return -1;
}

// sets a json value for the next index
int json_array_add_object(json_t *array, json_t *value, json_context_t *json_context)
{
  check(array != NULL, "array: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    int json_array_append_result = json_array_append(array, value);
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
  }
  else
  {
    int json_array_append_result = json_array_append(array, json_null());
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
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

  check(array != NULL, "array: NULL");
  check(json_context != NULL, "json_context: NULL");

  if (value != NULL)
  {
    int malloc_memcpy_string_result = malloc_memcpy_string(&value_copied, value);
    check(malloc_memcpy_string_result == 0, "malloc_memcpy_string_result: %d",
      malloc_memcpy_string_result);

    json_string_result = json_string(value_copied);
    check(json_string_result != NULL, "json_string_result: NULL");

    int json_array_append_result = json_array_append_new(array, json_string_result);
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);

    json_string_result = NULL;

    int array_add_result = array_add_string(
      &(json_context->strings),
      &(json_context->strings_allocated_count),
      &(json_context->strings_used_count),
      value_copied);

    check(array_add_result == 0, "array_add_result: %d",
      array_add_result);

    value_copied = NULL;
  }
  else
  {
    int json_array_append_result = json_array_append(array, json_null());
    check(json_array_append_result == 0, "json_array_append_result: %d",
      json_array_append_result);
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

  check(json != NULL, "json: NULL");
  check(string != NULL, "string: NULL");

  json_dumps_result = json_dumps(json, JSON_INDENT(2) | JSON_PRESERVE_ORDER);
  check(json_dumps_result != NULL, "json_dumps_result: NULL");

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
