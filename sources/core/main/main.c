#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_service.h"
#include "../../http/services/new_order_request_http.h"
#include "../../http/services/new_order_response_http.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  json_t *request_json = NULL;
  new_order_request_t *new_order_request = NULL;
  sqlite3 *sql_connection = NULL;
  new_order_response_t *new_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;
  json_t *response_json = NULL;
  char *response_string = NULL;

  char *request_string =
    "{\
       \"customer-name\":\"Alice\",\
       \"total\":100,\
       \"order-items\":[\
         {\
          \"name\":\"Nintendo\",\
          \"quantity\":2\
         },\
         {\
          \"name\":\"Dog\",\
          \"quantity\":1\
         }\
       ]\
     }";

  int json_parse_string_result = json_parse_string(request_string, &request_json);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  int new_order_request_json_parse_result = new_order_request_json_parse(request_json, &new_order_request);
  check(new_order_request_json_parse_result == 0, "new_order_request_json_parse_result: %d",
    new_order_request_json_parse_result);

  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  int new_order_service_result = new_order_service(
    sql_connection,
    new_order_request,
    &new_order_response,
    &validation_errors,
    &validation_errors_count);

  check(new_order_service_result == 0, "new_order_service_result: %d",
    new_order_service_result);

  int new_order_response_json_format_result = new_order_response_json_format(new_order_response, &response_json);
  check(new_order_response_json_format_result == 0, "new_order_response_json_format_result: %d",
    new_order_response_json_format_result);

  int json_to_string_result = json_to_string(response_json, &response_string);
  check(json_to_string_result == 0, "json_to_string_result: %d",
    json_to_string_result);

  printf("%s\n", response_string);

  free(response_string);
  json_free(response_json);
  validation_errors_free(validation_errors, validation_errors_count);
  new_order_response_free(new_order_response);
  sql_close_connection(sql_connection);
  new_order_request_free(new_order_request);
  json_free(request_json);

  return 0;

error:

  if (response_string != NULL) { free(response_string); }
  if (response_json != NULL) { json_free(response_json); }
  if (validation_errors != NULL) { validation_errors_free(validation_errors, validation_errors_count); }
  if (new_order_response != NULL) { new_order_response_free(new_order_response); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }
  if (new_order_request != NULL) { new_order_request_free(new_order_request); }
  if (request_json != NULL) { json_free(request_json); }

  return -1;
}
