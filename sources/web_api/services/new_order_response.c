#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/services/new_order_response.h"

// allocates a new order response
new_order_response_t *new_order_response_malloc(
  int *order_id)
{
  new_order_response_t *new_order_response = calloc(1, sizeof(new_order_response_t));
  check_mem(new_order_response);

  check_result(malloc_memcpy_int(&(new_order_response->order_id), order_id), 0);

  return new_order_response;

error:

  if (new_order_response != NULL) { new_order_response_free(new_order_response); }

  return NULL;
}

// frees a new order response
void new_order_response_free(new_order_response_t *new_order_response)
{
  if (new_order_response == NULL)
  {
    return;
  }

  if (new_order_response->order_id != NULL) { free(new_order_response->order_id); }

  free(new_order_response);
}
