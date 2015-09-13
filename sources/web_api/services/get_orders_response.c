#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/services/get_orders_response.h"
#include "../../web_api/services/get_orders_response_order.h"

// allocates a get orders response
get_orders_response_t *get_orders_response_malloc()
{
  get_orders_response_t *get_orders_response = calloc(1, sizeof(get_orders_response_t));
  check_mem(get_orders_response);

  return get_orders_response;

error:

  if (get_orders_response != NULL) { get_orders_response_free(get_orders_response); }

  return NULL;
}

// frees a get orders response
void get_orders_response_free(get_orders_response_t *get_orders_response)
{
  if (get_orders_response == NULL)
  {
    return;
  }

  if (get_orders_response->orders != NULL)
  {
    get_orders_response_orders_free(
      get_orders_response->orders,
      get_orders_response->orders_count);
  }

  free(get_orders_response);
}
