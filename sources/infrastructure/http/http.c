#include <fcgiapp.h>
#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/fastcgi/fastcgi.h"
#include "../../infrastructure/http/http.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/sql/sql.h"

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
  int *http_routes_used_count)
{
  http_route_t *http_route = NULL;

  check(service_parse_url != NULL, "service_parse_url: NULL");
  check(service_http != NULL, "service_http: NULL");
  check(http_routes != NULL, "http_routes: NULL");
  check(http_routes_allocated_count != NULL, "http_routes_allocated_count: NULL");
  check(http_routes_used_count != NULL, "http_routes_used_count: NULL");

  http_route = malloc(sizeof(http_route_t));
  check_mem(http_route);

  http_route->service_parse_url = service_parse_url;
  http_route->service_http = service_http;

  int array_add_pointer_result = array_add_pointer(
    (void ***)(http_routes),
    http_routes_allocated_count,
    http_routes_used_count,
    http_route);

  check(array_add_pointer_result == 0, "array_add_pointer_result: %d",
    array_add_pointer_result);

  http_route = NULL;

  return 0;

error:

  if (http_route != NULL) { free(http_route); }

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
int http_serve_request(FCGX_Request* request, http_route_t **http_routes, int http_routes_count)
{
  http_route_t *http_route = NULL;
  char **url_tokens = NULL;
  int url_tokens_count = 0;

  char *request_body = NULL;
  char *response_body = NULL;

  json_t *request_json = NULL;
  json_t *response_json = NULL;
  json_context_t *response_json_context = NULL;

  sqlite3 *sql_connection = NULL;

  check(request != NULL, "request: NULL");
  check(http_routes != NULL, "http_routes: NULL");

  int http_match_route_result = http_match_route(
    request,
    http_routes,
    http_routes_count,
    &http_route,
    &url_tokens,
    &url_tokens_count);

  check(http_match_route_result == 0, "http_match_route_result: %d",
    http_match_route_result);

  if (http_route == NULL)
  {
    int fastcgi_write_header_status_result = fastcgi_write_header(request->out, "Status", "404 Not Found", 1);
    check(fastcgi_write_header_status_result == 0, "fastcgi_write_header_status_result: %d",
      fastcgi_write_header_status_result);

    return 0;
  }

  int fastcgi_read_stream_result = fastcgi_read_stream(request->in, &request_body);
  check(fastcgi_read_stream_result == 0, "fastcgi_read_stream_result: %d",
    fastcgi_read_stream_result);

  int json_parse_string_result = json_parse_string(request_body, &request_json);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  if (request_json == NULL)
  {
    int fastcgi_write_header_status_result = fastcgi_write_header(request->out, "Status", "400 Bad Request", 1);
    check(fastcgi_write_header_status_result == 0, "fastcgi_write_header_status_result: %d",
      fastcgi_write_header_status_result);

    array_free_string(url_tokens, url_tokens_count);

    free(request_body);

    return 0;
  }

  response_json_context = json_context_malloc();
  check(response_json_context != NULL, "response_json_context: NULL");

  int sql_connection_open_result = sql_connection_open("/var/web_api/database.db", &sql_connection);
  check(sql_connection_open_result == 0, "sql_connection_open_result: %d",
    sql_connection_open_result);

  int service_http_result = http_route->service_http(
    sql_connection,
    url_tokens,
    url_tokens_count,
    request_json,
    &response_json,
    response_json_context);

  check(service_http_result == 0 || service_http_result == 1, "service_http_result: %d",
    service_http_result);

  sql_connection_close(sql_connection);

  sql_connection = NULL;

  int json_format_string_result = json_format_string(response_json, &response_body);
  check(json_format_string_result == 0, "json_format_string_result: %d",
    json_format_string_result);

  if (service_http_result == 0)
  {
    if (strcmp(response_body, "{}") != 0)
    {
      int fastcgi_write_status_result = fastcgi_write_header(request->out, "Status", "200: OK", 0);
      check(fastcgi_write_status_result == 0, "fastcgi_write_status_result: %d",
        fastcgi_write_status_result);

      int fastcgi_write_content_type_result = fastcgi_write_header(request->out, "Content-type", "application/json", 1);
      check(fastcgi_write_content_type_result == 0, "fastcgi_write_content_type_result: %d",
        fastcgi_write_content_type_result);

      int fastcgi_write_body_result = fastcgi_write_body(request->out, response_body);
      check(fastcgi_write_body_result == 0, "fastcgi_write_body_result: %d",
        fastcgi_write_body_result);
    }
    else
    {
      int fastcgi_write_status_result = fastcgi_write_header(request->out, "Status", "204: No Content", 1);
      check(fastcgi_write_status_result == 0, "fastcgi_write_status_result: %d",
        fastcgi_write_status_result);
    }
  }

  if (service_http_result == 1)
  {
    int fastcgi_write_status_result = fastcgi_write_header(request->out, "Status", "422: Unprocessable Entity", 0);
    check(fastcgi_write_status_result == 0, "fastcgi_write_status_result: %d",
      fastcgi_write_status_result);

    int fastcgi_write_content_type_result = fastcgi_write_header(request->out, "Content-type", "application/json", 1);
    check(fastcgi_write_content_type_result == 0, "fastcgi_write_content_type_result: %d",
      fastcgi_write_content_type_result);

    int fastcgi_write_body_result = fastcgi_write_body(request->out, response_body);
    check(fastcgi_write_body_result == 0, "fastcgi_write_body_result: %d",
      fastcgi_write_body_result);
  }

  json_context_free(response_json_context);
  json_free(response_json);
  json_free(request_json);

  free(response_body);
  free(request_body);

  array_free_string(url_tokens, url_tokens_count);

  return 0;

error:

  if (sql_connection != NULL) { sql_connection_close(sql_connection); }
  if (response_json_context != NULL) { json_context_free(response_json_context); }
  if (response_json != NULL) { json_free(response_json); }
  if (request_json != NULL) { json_free(request_json); }
  if (response_body != NULL) { free(response_body); }
  if (request_body != NULL) { free(request_body); }
  if (url_tokens != NULL) { array_free_string(url_tokens, url_tokens_count); }

  return -1;
}

// starts serving requests
int http_serve_requests(http_route_t **http_routes, int http_routes_count)
{
  FCGX_Request* request = NULL;

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
    int http_serve_request_result = http_serve_request(request, http_routes, http_routes_count);
    check(http_serve_request_result == 0, "http_serve_request_result: %d",
      http_serve_request_result);

    FCGX_Finish_r(request);
  }

  FCGX_Free(request, 1);
  free(request);

  return 0;

error:

  if (request != NULL)
  {
    FCGX_Free(request, 1);
    free(request);
  }

  return -1;
}

// frees an array of routes
void http_routes_free(http_route_t **http_routes, int count)
{
  if (http_routes == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    free(http_routes[i]);
  }

  free(http_routes);
}
