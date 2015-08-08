#include <fcgiapp.h>
#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/fastcgi/fastcgi.h"
#include "../../infrastructure/http/http.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/sql/sql.h"

// allocates a route
int http_route_malloc(
  http_route_t **http_route,
  int (*service_parse_url)(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count),
  int (*service_http)(sqlite3 *sql_connection, char **url_tokens, int url_tokens_count, json_t *body_request, json_t **body_response))
{
  http_route_t *http_route_return = NULL;

  check(service_parse_url != NULL, "service_parse_url: NULL");
  check(service_http != NULL, "service_http: NULL");

  http_route_return = malloc(sizeof(http_route_t));
  check_mem(http_route_return);

  http_route_return->service_parse_url = service_parse_url;
  http_route_return->service_http = service_http;

  *http_route = http_route_return;

  return 0;

error:

  if (http_route_return != NULL) { free(http_route_return); }

  return -1;
}

// searches for the route matching a request
int http_match_route(
  FCGX_Request* request,
  http_route_t **http_routes,
  int http_routes_count,
  http_route_t **http_route,
  char ***url_tokens,
  int *url_tokens_count)
{
  http_route_t *http_route_return = NULL;
  char **url_tokens_return = NULL;
  int url_tokens_count_return = 0;

  check(request != NULL, "request: NULL");
  check(http_routes != NULL, "http_routes: NULL");
  check(http_route != NULL, "http_route: NULL");
  check(url_tokens != NULL, "url_tokens: NULL");
  check(url_tokens_count != NULL, "url_tokens_count: NULL");

  char *method = FCGX_GetParam("REQUEST_METHOD", request->envp);
  char *url = FCGX_GetParam("REQUEST_URI", request->envp);

  for (int i = 0; i < http_routes_count; i++)
  {
    int matched = 0;

    int service_parse_url_result = http_routes[i]->service_parse_url(
      method,
      url,
      &matched,
      &url_tokens_return,
      &url_tokens_count_return);

    check(service_parse_url_result == 0, "service_parse_url_result: %d",
      service_parse_url_result);

    if (matched == 1)
    {
      http_route_return = http_routes[i];

      break;
    }

    url_tokens_return = NULL;
    url_tokens_count_return = 0;
  }

  *http_route = http_route_return;
  *url_tokens = url_tokens_return;
  *url_tokens_count = url_tokens_count_return;

  return 0;

error:

  if (url_tokens_return != NULL) { array_free_string(url_tokens_return, url_tokens_count_return); }

  return -1;
}

// serves a request
int http_serve_request(FCGX_Request* request, http_route_t *http_route, char **url_tokens, int url_tokens_count)
{
  char *request_body = NULL;
  char *response_body = NULL;

  json_t *request_json = NULL;
  json_t *response_json = NULL;

  sqlite3 *sql_connection = NULL;

  check(request != NULL, "request: NULL");

  int fastcgi_read_stream_result = fastcgi_read_stream(request->in, &request_body);
  check(fastcgi_read_stream_result == 0, "fastcgi_read_stream_result: %d",
    fastcgi_read_stream_result);

  int json_parse_string_result = json_parse_string(request_body, &request_json);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  int service_http_result = http_route->service_http(
    sql_connection,
    url_tokens,
    url_tokens_count,
    request_json,
    &response_json);

  check(service_http_result == 0, "service_http_result: %d",
    service_http_result);

  sql_close_connection(sql_connection);

  sql_connection = NULL;

  int json_format_string_result = json_format_string(response_json, &response_body);
  check(json_format_string_result == 0, "json_format_string_result: %d",
    json_format_string_result);

  FCGX_PutS("Content-type: text/html\r\n", request->out);
  FCGX_PutS("\r\n", request->out);
  FCGX_PutS(response_body, request->out);
  FCGX_PutS("\r\n", request->out);

  json_free(response_json);
  json_free(request_json);

  free(response_body);
  free(request_body);

  return 0;

error:

  if (sql_connection != NULL) { sql_close_connection(sql_connection); }
  if (response_json != NULL) { json_free(response_json); }
  if (request_json != NULL) { json_free(request_json); }
  if (response_body != NULL) { free(response_body); }
  if (request_body != NULL) { free(request_body); }

  return -1;
}

// starts serving requests
int http_serve_requests(http_route_t **http_routes, int http_routes_count)
{
  FCGX_Request* request = NULL;
  char **url_tokens = NULL;
  int url_tokens_count = 0;

  check(http_routes != NULL, "http_routes: NULL");

  int init_result = FCGX_Init();
  check(init_result == 0, "init_result: %d",
    init_result);

  request = malloc(sizeof(FCGX_Request));
  check_mem(request);

  int init_request_result = FCGX_InitRequest(request, 0, 0);
  check(init_request_result == 0, "init_request_result: %d",
    init_request_result);

  while (FCGX_Accept_r(request) == 0)
  {
    http_route_t *http_route = NULL;

    int http_match_route_result = http_match_route(
      request,
      http_routes,
      http_routes_count,
      &http_route,
      &url_tokens,
      &url_tokens_count);

    check(http_match_route_result == 0, "http_match_route_result: %d",
      http_match_route_result);

    if (http_route != NULL)
    {
      int http_serve_request_result = http_serve_request(request, http_route, url_tokens, url_tokens_count);
      check(http_serve_request_result == 0, "http_serve_request_result: %d",
        http_serve_request_result);
    }

    array_free_string(url_tokens, url_tokens_count);

    url_tokens = NULL;
    url_tokens_count = 0;

    FCGX_Finish_r(request);
  }

  FCGX_Free(request, 1);
  free(request);

  return 0;

error:

  if (url_tokens != NULL) { array_free_string(url_tokens, url_tokens_count); }

  if (request != NULL)
  {
    FCGX_Free(request, 1);
    free(request);
  }

  return -1;
}
