#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../http/services/new_order_service_http.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  sqlite3 *sql_connection = NULL;
  json_t *json_request = NULL;
  json_t *json_response = NULL;

  char *json_request_string = "{\"customer-name\":\"Alice\",\"order-items\":[{\"name\":\"Pirate hat\",\"quantity\":1.25}],\"total\":100}";
  char *json_response_string = NULL;

  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  int json_parse_string_result = json_parse_string(json_request_string, &json_request);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  int new_order_service_json_result = new_order_service_json(sql_connection, NULL, 0, json_request, &json_response);
  check(new_order_service_json_result == 0, "new_order_service_json_result: %d",
    new_order_service_json_result);

  int json_format_string_result = json_format_string(json_response, &json_response_string);
  check(json_format_string_result == 0, "json_format_string_result: %d",
    json_format_string_result);

  printf("%s\n", json_response_string);

  sql_close_connection(sql_connection);
  json_free(json_request);
  json_free(json_response);
  free(json_response_string);

  return 0;

error:

  if (sql_connection != NULL) { sql_close_connection(sql_connection); }
  if (json_request != NULL) { json_free(json_request); }
  if (json_response != NULL) { json_free(json_response); }
  if (json_response_string != NULL) { free(json_response_string); }

  return -1;
}
