#ifndef NEW_ORDER_SERVICE_HTTP_H
#define NEW_ORDER_SERVICE_HTTP_H

// parses a url and returns whether it matches the new order service
int new_order_service_http_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count);

// executes the new order service
int new_order_service_http(char **url_tokens, int url_tokens_count);

#endif
