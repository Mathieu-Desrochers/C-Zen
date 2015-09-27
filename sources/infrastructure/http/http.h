#ifndef HTTP_H
#define HTTP_H

#include <jansson.h>
#include <sqlite3.h>

#include "../../infrastructure/json/json.h"

// signature of a service parse url function
typedef int (*http_service_parse_url_t)(
  char *method,
  char *url,
  int *matched,
  char ***url_tokens,
  int *url_tokens_count);

// signature of a service http function
typedef int (*http_service_http_t)(
  sqlite3 *sql_connection,
  char **url_tokens,
  int url_tokens_count,
  json_t *request_json,
  json_t **response_json,
  json_context_t *response_json_context);

// represents a route
typedef struct http_route_t
{
  http_service_parse_url_t service_parse_url;
  http_service_http_t service_http;
} http_route_t;

// registers a route
int http_route_register(
  http_service_parse_url_t service_parse_url,
  http_service_http_t service_http,
  http_route_t ***http_routes,
  int *http_routes_allocated_count,
  int *http_routes_used_count);

// starts serving requests
int http_serve_requests(http_route_t **http_routes, int http_routes_count);

// frees an array of routes
void http_routes_free(http_route_t **http_routes, int count);

#endif
