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
int json_get_int(json_t *object, char *key, int **value)
{
  int *value_return = NULL;

  check(object != NULL, "object: NULL");

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
int json_get_string(json_t *object, char *key, char **value)
{
  char *value_return = NULL;

  check(object != NULL, "object: NULL");

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

// frees a parsed json string
void json_parse_string_free(json_t *json)
{
  if (json == NULL)
  {
    return;
  }

  json_decref(json);
}
