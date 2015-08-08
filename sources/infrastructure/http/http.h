#ifndef HTTP_H
#define HTTP_H

#include <jansson.h>
#include <sqlite3.h>

// represents a route
typedef struct http_route_t
{
  int (*service_parse_url)(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count);
  int (*service_http)(sqlite3 *sql_connection, char **url_tokens, int url_tokens_count, json_t *body_request, json_t **body_response);
} http_route_t;

// allocates a route
int http_route_malloc(
  http_route_t **http_route,
  int (*service_parse_url)(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count),
  int (*service_http)(sqlite3 *sql_connection, char **url_tokens, int url_tokens_count, json_t *body_request, json_t **body_response));

// starts serving requests
int http_serve_requests(http_route_t **http_routes, int http_routes_count);

#endif
