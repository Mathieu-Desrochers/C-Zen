#ifndef NEW_ORDER_SERVICE_HTTP_H
#define NEW_ORDER_SERVICE_HTTP_H

#include <jansson.h>
#include <sqlite3.h>

// parses a url and returns whether it matches the new order service
int new_order_service_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count);

// executes the new order service
int new_order_service_json(sqlite3 *sql_connection, char **url_tokens, int url_tokens_count, json_t *body_request, json_t **body_response);

#endif
