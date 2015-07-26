#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"
#include "../../core/services/new_order_service.h"
#include "../../http/new_order_request_json.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  char *json_string = "{\"customer-name\":\"Alice\",\"order-items\":[{\"name\":\"Pirate hat\",\"quantity\":1.25}],\"total\":100}";
  json_t *json = NULL;

  sqlite3 *sql_connection = NULL;
  new_order_request_t *new_order_request = NULL;
  new_order_response_t *new_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;

  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  int json_parse_string_result = json_parse_string(json_string, &json);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  new_order_request_json_parse(json, &new_order_request);
  check(new_order_request != NULL, "new_order_request: NULL");

  int new_order_service_result = new_order_service(
    sql_connection,
    new_order_request,
    &new_order_response,
    &validation_errors,
    &validation_errors_count);

  check(new_order_service_result == 0, "new_order_service_result: %d",
    new_order_service_result);

  sql_close_connection(sql_connection);
  json_parse_string_free(json);
  new_order_request_free(new_order_request);
  new_order_response_free(new_order_response);
  free(validation_errors);

  return 0;

error:

  if (sql_connection != NULL) { sql_close_connection(sql_connection); }
  if (json != NULL) { json_parse_string_free(json); }
  if (new_order_request != NULL) { new_order_request_free(new_order_request); }
  if (new_order_response != NULL) { new_order_response_free(new_order_response); }
  if (validation_errors != NULL) { free(validation_errors); }

  return -1;
}
