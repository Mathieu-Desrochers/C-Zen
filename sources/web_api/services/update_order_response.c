#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/services/update_order_response.h"

// allocates an update order response
update_order_response_t *update_order_response_malloc()
{
  update_order_response_t *update_order_response = calloc(1, sizeof(update_order_response_t));
  check_mem(update_order_response);

  return update_order_response;

error:

  if (update_order_response != NULL) { update_order_response_free(update_order_response); }

  return NULL;
}

// frees an update order response
void update_order_response_free(update_order_response_t *update_order_response)
{
  if (update_order_response == NULL)
  {
    return;
  }

  free(update_order_response);
}
