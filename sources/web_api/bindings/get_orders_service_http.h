#ifndef GET_ORDERS_SERVICE_HTTP_H
#define GET_ORDERS_SERVICE_HTTP_H

#include <jansson.h>
#include <sqlite3.h>

#include "../../infrastructure/json/json.h"

// parses a url and returns whether it matches the get orders service
int get_orders_service_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count);

// executes the get orders service
int get_orders_service_http(
  sqlite3 *sql_connection,
  char **url_tokens,
  int url_tokens_count,
  json_t *request_json,
  json_t **response_json,
  json_context_t *response_json_context);

#endif
