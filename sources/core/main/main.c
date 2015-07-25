#include <stdio.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"

int main()
{
  char *json_string = "[1, \"2\", []]";
  json_t *json_parsed = NULL;
  json_t *value = NULL;

  int json_parse_string_result = json_parse_string(json_string, &json_parsed);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  int json_array_get_array_result = json_array_get_array(json_parsed, 2, &value);
  check(json_array_get_array_result == 0, "json_array_get_array_result: %d",
    json_array_get_array_result);

  printf("%p\n", value);

  json_parse_string_free(json_parsed);

  return 0;

error:

  if (json_parsed != NULL) { json_parse_string_free(json_parsed); }

  return -1;
}
