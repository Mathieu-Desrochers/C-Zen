#include "../../http/services/new_order_service_http.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/regex/regex.h"

// parses a url and returns whether it matches the new order service
int new_order_service_http_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count)
{
  int matched_return = 0;
  char **url_tokens_return = NULL;
  int url_tokens_count_return = 0;

  int regex_match_result = regex_match(
    "^/new-order$",
    url,
    &matched_return,
    &url_tokens_return,
    &url_tokens_count_return);

  check(regex_match_result == 0, "regex_match_result: %d",
    regex_match_result);

  *matched = matched_return;
  *url_tokens = url_tokens_return;
  *url_tokens_count = url_tokens_count_return;

  return 0;

error:

  if (url_tokens_return != NULL) { array_free_string(url_tokens_return, url_tokens_count_return); }

  return -1;
}

// executes the new order service
int new_order_service_http(char **url_tokens, int url_tokens_count)
{
  return 0;
}
