#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../web_api/bindings/update_order_response_http.h"
#include "../../web_api/services/update_order_response.h"

// formats an update order response
int update_order_response_http_format(update_order_response_t *update_order_response, json_t **json, json_context_t *json_context)
{
  json_t *json_return = NULL;

  check_not_null(update_order_response);
  check_not_null(json);
  check_not_null(json_context);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}
