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
  http_service_parse_url_t service_parse_url,
  http_service_http_t service_http,
  http_route_t ***http_routes,
  int *http_routes_allocated_count,
  int *http_routes_used_count)
{
  http_route_t *http_route = NULL;

  check_not_null(service_parse_url);
  check_not_null(service_http);
  check_not_null(http_routes);
  check_not_null(http_routes_allocated_count);
  check_not_null(http_routes_used_count);

  http_route = malloc(sizeof(http_route_t));
  check_mem(http_route);

  http_route->service_parse_url = service_parse_url;
  http_route->service_http = service_http;

  check_result(
    array_add_pointer(
      (void ***)(http_routes),
      http_routes_allocated_count,
      http_routes_used_count,
      http_route),
    0);

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

  check_not_null(request);
  check_not_null(http_routes);
  check_not_null(http_route);
  check_not_null(url_tokens);
  check_not_null(url_tokens_count);

  char *method = FCGX_GetParam("REQUEST_METHOD", request->envp);
  char *url = FCGX_GetParam("REQUEST_URI", request->envp);

  check_not_null(method);
  check_not_null(url);

  for (int i = 0; i < http_routes_count; i++)
  {
    int matched = 0;

    http_service_parse_url_t service_parse_url = http_routes[i]->service_parse_url;

    check_result(
      service_parse_url(
        method,
        url,
        &matched,
        &url_tokens_return,
        &url_tokens_count_return),
      0);

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

  check_not_null(request);
  check_not_null(http_routes);

  check_result(
    http_match_route(
      request,
      http_routes,
      http_routes_count,
      &http_route,
      &url_tokens,
      &url_tokens_count),
    0);

  if (http_route == NULL)
  {
    check_result(fastcgi_write_header(request->out, "Status", "404 Not Found", 1), 0);
    return 0;
  }

  check_result(fastcgi_read_stream(request->in, &request_body), 0);

  if (request_body != NULL)
  {
    check_result(json_parse_string(request_body, &request_json), 0);

    if (request_json == NULL)
    {
      check_result(fastcgi_write_header(request->out, "Status", "400 Bad Request", 1), 0);
      array_free_string(url_tokens, url_tokens_count);
      free(request_body);

      return 0;
    }
  }

  response_json_context = json_context_malloc();
  check_not_null(response_json_context);

  check_result(sql_connection_open("/var/web_api/database.db", &sql_connection), 0);
  check_result(sql_transaction_begin(sql_connection), 0);

  http_service_http_t service_http = http_route->service_http;

  int service_http_result = service_http(
    sql_connection,
    url_tokens,
    url_tokens_count,
    request_json,
    &response_json,
    response_json_context);

  if (service_http_result == 0)
  {
    check_result(sql_transaction_commit(sql_connection), 0);
  }
  else
  {
    check_result(sql_transaction_rollback(sql_connection), 0);
  }

  sql_connection_close(sql_connection);
  sql_connection = NULL;

  if (service_http_result == 0)
  {
    if (response_json != NULL)
    {
      check_result(fastcgi_write_header(request->out, "Status", "200: OK", 0), 0);
      check_result(fastcgi_write_header(request->out, "Content-type", "application/json", 1), 0);
      check_result(json_format_string(response_json, &response_body), 0);
      check_result(fastcgi_write_body(request->out, response_body), 0);
    }
    else
    {
      check_result(fastcgi_write_header(request->out, "Status", "204: No Content", 1), 0);
    }
  }
  else
  {
    check_result(fastcgi_write_header(request->out, "Status", "422: Unprocessable Entity", 0), 0);
    check_result(fastcgi_write_header(request->out, "Content-type", "application/json", 1), 0);
    check_result(json_format_string(response_json, &response_body), 0);
    check_result(fastcgi_write_body(request->out, response_body), 0);
  }

  json_context_free(response_json_context);
  json_free(response_json);
  json_free(request_json);

  free(response_body);
  free(request_body);

  array_free_string(url_tokens, url_tokens_count);

  return 0;

error:

  if (sql_connection != NULL)
  {
    sql_transaction_rollback(sql_connection);
    sql_connection_close(sql_connection);
  }

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

  check_not_null(http_routes);

  check_result(FCGX_Init(), 0);

  request = malloc(sizeof(FCGX_Request));
  check_mem(request);

  check_result(FCGX_InitRequest(request, 0, 0), 0);

  while (FCGX_Accept_r(request) == 0)
  {
    check_result(http_serve_request(request, http_routes, http_routes_count), 0);
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
