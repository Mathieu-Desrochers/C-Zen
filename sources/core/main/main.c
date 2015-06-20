#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_request_order_item.h"
#include "../../core/services/new_order_response.h"
#include "../../core/services/new_order_service.h"
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

  char *customer_name = "Alice";
  int total = 100;

  new_order_request_t *new_order_request = new_order_request_malloc(
    customer_name,
    &total);

  char *name_1 = "Pizza";
  double quantity_1 = 3.8;

  new_order_request_order_item_t *new_order_request_order_item_1 = new_order_request_order_item_malloc(
    name_1,
    &quantity_1);

  char *name_2 = "Pirate hat";
  double quantity_2 = 6;

  new_order_request_order_item_t *new_order_request_order_item_2 = new_order_request_order_item_malloc(
    name_2,
    &quantity_2);

  new_order_request->order_items = malloc(sizeof(new_order_request_order_item_t *) * 2);
  new_order_request->order_items[0] = new_order_request_order_item_1;
  new_order_request->order_items[1] = new_order_request_order_item_2;
  new_order_request->order_items_count = 2;

  new_order_response_t *new_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int count = 0;

  int result = new_order_service(sql_connection, new_order_request, &new_order_response, &validation_errors, &count);
  check(result == 0, "result: %d", result);

  printf("count: %d\n", count);

  for (int i = 0; i < count; i++)
  {
    print_error(validation_errors[i]);
  }

  validation_errors_free(validation_errors, count);
  new_order_response_free(new_order_response);
  new_order_request_free(new_order_request);
  sql_close_connection(sql_connection);

  return 0;

error:

  if (validation_errors != NULL) { validation_errors_free(validation_errors, count); }
  if (new_order_response != NULL) {new_order_response_free(new_order_response); }
  if (new_order_request != NULL) {new_order_request_free(new_order_request); }
  if (sql_connection != NULL) {sql_close_connection(sql_connection); }

  return -1;
}
