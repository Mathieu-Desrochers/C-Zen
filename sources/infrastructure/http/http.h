#ifndef HTTP_H
#define HTTP_H

#include <jansson.h>
#include <sqlite3.h>

#include "../../infrastructure/json/json.h"

// represents a route
typedef struct http_route_t
{
  int (*service_parse_url)(
    char *method,
    char *url,
    int *matched,
    char ***url_tokens,
    int *url_tokens_count);
  int (*service_http)(
    sqlite3 *sql_connection,
    char **url_tokens,
    int url_tokens_count,
    json_t *request_json,
    json_t **response_json,
    json_context_t *response_json_context);
} http_route_t;

// registers a route
int http_route_register(
  int (*service_parse_url)(
    char *method,
    char *url,
    int *matched,
    char ***url_tokens,
    int *url_tokens_count),
  int (*service_http)(
    sqlite3 *sql_connection,
    char **url_tokens,
    int url_tokens_count,
    json_t *request_json,
    json_t **response_json,
    json_context_t *response_json_context),
  http_route_t ***http_routes,
  int *http_routes_allocated_count,
  int *http_routes_used_count);

// starts serving requests
int http_serve_requests(http_route_t **http_routes, int http_routes_count);

// frees an array of routes
void http_routes_free(http_route_t **http_routes, int count);

#endif
