#include <stdio.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"

int main()
{
  char *json_string = NULL;
  json_t *json = NULL;
  json_t *json2 = NULL;

  json = json_object_malloc();
  check(json != NULL, "json: NULL");

  json2 = json_object_malloc();
  check(json2 != NULL, "json2: NULL");

  int json_object_set_child_result = json_object_set_object(json, "child", json2);
  check(json_object_set_child_result == 0, "json_object_set_child_result: %d",
    json_object_set_child_result);

  char *value = "Alice";

  int json_object_set_number_result = json_object_set_string(json2, "number", value);
  check(json_object_set_number_result == 0, "json_object_set_number_result %d",
    json_object_set_number_result);

  int json_to_string_result = json_to_string(json, &json_string);
  check(json_to_string_result == 0, "json_to_string_result: %d",
    json_to_string_result);

  printf("%s\n", json_string);

  free(json_string);
  json_free(json2);
  json_free(json);

  return 0;

error:

  if (json_string != NULL) { free(json_string); }
  if (json2 != NULL) { json_free(json2); }
  if (json != NULL) { json_free(json); }

  return -1;
}
