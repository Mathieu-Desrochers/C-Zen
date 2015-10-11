#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/services/get_order_response.h"
#include "../../web_api/services/get_order_response_order_item.h"

// allocates a get order response
get_order_response_t *get_order_response_malloc(
  int *order_id,
  char *customer_name,
  time_t *placed_on_date_time,
  int *total)
{
  get_order_response_t *get_order_response = calloc(1, sizeof(get_order_response_t));
  check_mem(get_order_response);

  check_result(malloc_memcpy_int(&(get_order_response->order_id), order_id), 0);
  check_result(malloc_memcpy_string(&(get_order_response->customer_name), customer_name), 0);
  check_result(malloc_memcpy_time(&(get_order_response->placed_on_date_time), placed_on_date_time), 0);
  check_result(malloc_memcpy_int(&(get_order_response->total), total), 0);

  return get_order_response;

error:

  if (get_order_response != NULL) { get_order_response_free(get_order_response); }

  return NULL;
}

// frees a get order response
void get_order_response_free(get_order_response_t *get_order_response)
{
  if (get_order_response == NULL)
  {
    return;
  }

  if (get_order_response->order_id != NULL) { free(get_order_response->order_id); }
  if (get_order_response->customer_name != NULL) { free(get_order_response->customer_name); }
  if (get_order_response->placed_on_date_time != NULL) { free(get_order_response->placed_on_date_time); }
  if (get_order_response->total != NULL) { free(get_order_response->total); }

  if (get_order_response->order_items != NULL)
  {
    get_order_response_order_items_free(
      get_order_response->order_items,
      get_order_response->order_items_count);
  }

  free(get_order_response);
}
