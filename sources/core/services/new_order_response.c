#include <stdlib.h>

#include "../../core/services/new_order_response.h"
#include "../../infrastructure/dbg/dbg.h"

// allocates a new order response
new_order_response_t *new_order_response_malloc(
  int order_id)
{
  new_order_response_t *new_order_response = malloc(sizeof(new_order_response_t));
  check_mem(new_order_response);

  new_order_response->order_id = order_id;

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

  free(new_order_response);
}

// frees an array of new order responses
void new_order_responses_free(new_order_response_t **new_order_responses, int count)
{
  if (new_order_responses == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    new_order_response_free(new_order_responses[i]);
  }

  free(new_order_responses);
}
