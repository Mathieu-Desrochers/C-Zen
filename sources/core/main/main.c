#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/update_order_request.h"
#include "../../core/services/update_order_request_order_item.h"
#include "../../core/services/update_order_response.h"
#include "../../core/services/update_order_service.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

void print_validation_error(validation_error_t *validation_error)
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
  sqlite3 *sql_connection = NULL;
  update_order_request_t *update_order_request = NULL;
  update_order_request_order_item_t *update_order_request_order_item_1 = NULL;
  update_order_request_order_item_t *update_order_request_order_item_2 = NULL;
  update_order_request_order_item_t *update_order_request_order_item_3 = NULL;
  update_order_response_t *update_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;

  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  int order_id = 1;
  char *customer_name = "Alice";
  int total = 0;

  update_order_request = update_order_request_malloc(
    &order_id,
    customer_name,
    &total);

  check_mem(update_order_request);

  int order_item_id = 0;
  char *name = NULL;
  double quantity = 0.0;
  int allocated_order_items_count = 0;

  order_item_id = 1;
  name = "Pirate hat";
  quantity = 5;

  update_order_request_order_item_1 = update_order_request_order_item_malloc(
    &order_item_id,
    name,
    &quantity);

  check_mem(update_order_request_order_item_1);

  int array_add_result = array_add_pointer(
    (void ***)&(update_order_request->order_items),
    &allocated_order_items_count,
    &(update_order_request->order_items_count),
    update_order_request_order_item_1);

  check(array_add_result == 0, "array_add_result: %d",
    array_add_result);

  order_item_id = 2;
  name = "Pirate hat";
  quantity = 5;

  update_order_request_order_item_2 = update_order_request_order_item_malloc(
    &order_item_id,
    name,
    &quantity);

  check_mem(update_order_request_order_item_2);

  array_add_result = array_add_pointer(
    (void ***)&(update_order_request->order_items),
    &allocated_order_items_count,
    &(update_order_request->order_items_count),
    update_order_request_order_item_2);

  check(array_add_result == 0, "array_add_result: %d",
    array_add_result);

  order_item_id = 3;
  name = "Pirate hats";
  quantity = 55;

  update_order_request_order_item_3 = update_order_request_order_item_malloc(
    &order_item_id,
    name,
    &quantity);

  check_mem(update_order_request_order_item_3);

  array_add_result = array_add_pointer(
    (void ***)&(update_order_request->order_items),
    &allocated_order_items_count,
    &(update_order_request->order_items_count),
    update_order_request_order_item_3);

  check(array_add_result == 0, "array_add_result: %d",
    array_add_result);

  int update_order_service_result = update_order_service(
    sql_connection,
    update_order_request,
    &update_order_response,
    &validation_errors,
    &validation_errors_count);

  check(update_order_service_result == 0, "update_order_service_result: %d",
    update_order_service_result);

  printf("validation_errors_count: %d\n", validation_errors_count);

  for (int i = 0; i < validation_errors_count; i++)
  {
    print_validation_error(validation_errors[i]);
  }

  validation_errors_free(validation_errors, validation_errors_count);
  update_order_response_free(update_order_response);
  update_order_request_free(update_order_request);
  sql_close_connection(sql_connection);

  return 0;

error:

  if (validation_errors != NULL) { validation_errors_free(validation_errors, validation_errors_count); }
  if (update_order_response != NULL) {update_order_response_free(update_order_response); }
  if (update_order_request != NULL) {update_order_request_free(update_order_request); }
  if (sql_connection != NULL) {sql_close_connection(sql_connection); }

  return -1;
}
