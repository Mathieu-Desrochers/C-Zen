#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/http/http.h"
#include "../../infrastructure/json/json.h"
#include "../../web_api/bindings/new_order_service_http.h"

int main()
{
  http_route_t **http_routes = NULL;
  int http_routes_allocated_count = 0;
  int http_routes_used_count = 0;

  http_route_t *http_route = NULL;

  http_routes = malloc(sizeof(http_route_t *) * 1);
  check_mem(http_routes);

  http_route = http_route_malloc(
    new_order_service_parse_url,
    new_order_service_http);

  check(http_route != NULL, "http_route: NULL");

  int array_add_pointer_result = array_add_pointer(
    (void ***)(&http_routes),
    &http_routes_allocated_count,
    &http_routes_used_count,
    http_route);

  check(array_add_pointer_result == 0, "array_add_pointer_result: %d",
    array_add_pointer_result);

  http_route = NULL;

  int http_serve_requests_result = http_serve_requests(http_routes, 1);
  check(http_serve_requests_result == 0, "http_serve_requests_result: %d",
    http_serve_requests_result);

  free(http_routes[0]);
  free(http_route);

  return 0;

error:

  if (http_routes != NULL)
  {
    free(http_routes[0]);
    free(http_routes);
  }

  return -1;
}
