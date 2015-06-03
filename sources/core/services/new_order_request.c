#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_request_order_item.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"
#include "../../infrastructure/validation/validation.h"

#define NEW_ORDER_REQUEST_CUSTOMER 1

// allocates a new order request
new_order_request_t *new_order_request_malloc(
  char *customer,
  int total)
{
  new_order_request_t *new_order_request = malloc(sizeof(new_order_request_t));
  check_mem(new_order_request);

  new_order_request->customer = strdup(customer);
  check_mem(new_order_request->customer);

  new_order_request->order_items = NULL;
  new_order_request->order_items_count = 0;
  new_order_request->total = total;

  return new_order_request;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }

  return NULL;
}

// validates a new order request
int new_order_request_validate(new_order_request_t *new_order_request, validation_error_t ***validation_errors, int *count)
{
  validation_error_t **validation_errors_return = NULL;

  int allocated_errors_count = 0;
  int detected_errors_count = 0;

  check(new_order_request != NULL, "new_order_request: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(count != NULL, "count: NULL");

  int validate_customer_result = validate_string(new_order_request->customer, 1, 1, 100);
  if (validate_customer_result != 0)
  {
    int validation_errors_add_result = validation_errors_add(
      &validation_errors_return, &allocated_errors_count, &detected_errors_count,
      NEW_ORDER_REQUEST_CUSTOMER, -1, validate_customer_result);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }

  *validation_errors = validation_errors_return;
  *count = detected_errors_count;

  return 0;

error:

  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, detected_errors_count); }

  return -1;
}

// frees a new order request
void new_order_request_free(new_order_request_t *new_order_request)
{
  if (new_order_request == NULL)
  {
    return;
  }

  if (new_order_request->customer != NULL) { free(new_order_request->customer); }
  if (new_order_request->order_items != NULL)
  {
    new_order_request_order_items_free(new_order_request->order_items, new_order_request->order_items_count);
  }

  free(new_order_request);
}
