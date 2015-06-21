#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/update_order_request.h"
#include "../../core/services/update_order_request_order_item.h"
#include "../../core/services/update_order_response.h"
#include "../../core/services/update_order_service.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

void print_error(validation_error_t *validation_error)
{
  validation_path_t *validation_path = validation_error->validation_path;

  while (validation_path != NULL)
  {
    printf("property: %d\tindex: %d\t", validation_path->property, validation_path->index);
    validation_path = validation_path->next;
  }

  printf("error_code: %d\n", validation_error->error_code);
}

int main()
{
  sqlite3 *sql_connection;
  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  int order_id = 199;
  char *customer_name = "Alice";
  int total = 100;

  update_order_request_t *update_order_request = update_order_request_malloc(
    &order_id,
    customer_name,
    &total);

  update_order_response_t *update_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int count = 0;

  int result = update_order_service(sql_connection, update_order_request, &update_order_response, &validation_errors, &count);
  check(result == 0, "result: %d", result);

  printf("count: %d\n", count);

  for (int i = 0; i < count; i++)
  {
    print_error(validation_errors[i]);
  }

  validation_errors_free(validation_errors, count);
  update_order_response_free(update_order_response);
  update_order_request_free(update_order_request);
  sql_close_connection(sql_connection);

  return 0;

error:

  if (validation_errors != NULL) { validation_errors_free(validation_errors, count); }
  if (update_order_response != NULL) {update_order_response_free(update_order_response); }
  if (update_order_request != NULL) {update_order_request_free(update_order_request); }
  if (sql_connection != NULL) {sql_close_connection(sql_connection); }

  return -1;
}
