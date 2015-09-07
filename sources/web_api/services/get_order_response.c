#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/services/get_order_response.h"

// allocates a get order response
get_order_response_t *get_order_response_malloc(
  int *order_id,
  char *customer_name,
  time_t *placed_on_date_time,
  int *total)
{
  get_order_response_t *get_order_response = malloc(sizeof(get_order_response_t));
  check_mem(get_order_response);

  int malloc_memcpy_order_id_result = malloc_memcpy_int(&(get_order_response->order_id), order_id);
  check(malloc_memcpy_order_id_result == 0, "malloc_memcpy_order_id_result: %d",
    malloc_memcpy_order_id_result);

  int malloc_memcpy_customer_name_result = malloc_memcpy_string(&(get_order_response->customer_name), customer_name);
  check(malloc_memcpy_customer_name_result == 0, "malloc_memcpy_customer_name_result: %d",
    malloc_memcpy_customer_name_result);

  int malloc_memcpy_placed_on_date_time_result = malloc_memcpy_time(&(get_order_response->placed_on_date_time), placed_on_date_time);
  check(malloc_memcpy_placed_on_date_time_result == 0, "malloc_memcpy_placed_on_date_time_result: %d",
    malloc_memcpy_placed_on_date_time_result);

  int malloc_memcpy_total_result = malloc_memcpy_int(&(get_order_response->total), total);
  check(malloc_memcpy_total_result == 0, "malloc_memcpy_total_result: %d",
    malloc_memcpy_total_result);

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

  free(get_order_response);
}
