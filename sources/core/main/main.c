#include <stdio.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"

int main()
{
  char *json_string = "{\"name\":1}";
  json_t *json_parsed = NULL;
  int *value = NULL;

  int json_parse_string_result = json_parse_string(json_string, &json_parsed);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  int json_get_int_result = json_get_int(json_parsed, "name", &value);
  check(json_get_int_result == 0, "json_get_int_result: %d",
    json_get_int_result);

  printf("%p\n", value);

  free(value);
  json_parse_string_free(json_parsed);

  return 0;

error:

  if (value != NULL) { free(value); }
  if (json_parsed != NULL) { json_parse_string_free(json_parsed); }

  return -1;
}
