#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/http/http.h"
#include "../../infrastructure/json/json.h"
#include "../../http/services/new_order_service_http.h"

int main()
{
  http_route_t **http_routes = NULL;
  int allocated_http_routes = 0;
  int used_http_routes = 0;

  http_route_t *http_route = NULL;

  http_routes = malloc(sizeof(http_route_t *) * 1);
  check_mem(http_routes);

  int http_route_malloc_result = http_route_malloc(
    &http_route,
    new_order_service_parse_url,
    new_order_service_http);

  check(http_route_malloc_result == 0, "http_route_malloc_result: %d",
    http_route_malloc_result);

  int array_add_pointer_result = array_add_pointer(
    (void ***)(&http_routes),
    &allocated_http_routes,
    &used_http_routes,
    http_route);

  check(array_add_pointer_result == 0, "array_add_pointer_result: %d",
    array_add_pointer_result);

  http_route = NULL;

  int http_serve_requests_result = http_serve_requests(http_routes, 1);
  check(http_serve_requests_result == 0, "http_serve_requests_result: %d",
    http_serve_requests_result);

  return 0;

error:

  return -1;
}
