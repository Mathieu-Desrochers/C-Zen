#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/http/http.h"
#include "../../infrastructure/json/json.h"
#include "../../web_api/bindings/get_order_service_http.h"
#include "../../web_api/bindings/get_orders_service_http.h"
#include "../../web_api/bindings/new_order_service_http.h"
#include "../../web_api/bindings/update_order_service_http.h"

int main()
{
  http_route_t **http_routes = NULL;
  int http_routes_allocated_count = 0;
  int http_routes_used_count = 0;

  int http_route_register_result = 0;

  http_route_register_result = http_route_register(
    get_order_service_parse_url,
    get_order_service_http,
    &http_routes,
    &http_routes_allocated_count,
    &http_routes_used_count);

  check(http_route_register_result == 0, "http_route_register_result: %d",
    http_route_register_result);

  http_route_register_result = http_route_register(
    get_orders_service_parse_url,
    get_orders_service_http,
    &http_routes,
    &http_routes_allocated_count,
    &http_routes_used_count);

  check(http_route_register_result == 0, "http_route_register_result: %d",
    http_route_register_result);

  http_route_register_result = http_route_register(
    new_order_service_parse_url,
    new_order_service_http,
    &http_routes,
    &http_routes_allocated_count,
    &http_routes_used_count);

  check(http_route_register_result == 0, "http_route_register_result: %d",
    http_route_register_result);

  http_route_register_result = http_route_register(
    update_order_service_parse_url,
    update_order_service_http,
    &http_routes,
    &http_routes_allocated_count,
    &http_routes_used_count);

  check(http_route_register_result == 0, "http_route_register_result: %d",
    http_route_register_result);

  int http_serve_requests_result = http_serve_requests(http_routes, http_routes_used_count);
  check(http_serve_requests_result == 0, "http_serve_requests_result: %d",
    http_serve_requests_result);

  http_routes_free(http_routes, http_routes_used_count);

  return 0;

error:

  if (http_routes != NULL)
  {
    http_routes_free(http_routes, http_routes_used_count);
  }

  return -1;
}
