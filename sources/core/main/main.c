#include <stdio.h>
#include <stdlib.h>

#include "../../http/services/new_order_service_http.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"

int main()
{
  int matched = 0;
  char **url_tokens = NULL;
  int url_tokens_count = 0;

  int parse_url_result = new_order_service_http_parse_url("GET", "/new-order", &matched, &url_tokens, &url_tokens_count);
  check(parse_url_result == 0, "parse_url_result: %d",
    parse_url_result);

  printf("matched: %d\n", matched);

  for (int i = 0; i < url_tokens_count; i++)
  {
    printf("url_tokens[%d]: %s\n", i, url_tokens[i]);
  }

  array_free_string(url_tokens, url_tokens_count);

  return 0;

error:

  if (url_tokens != NULL) { array_free_string(url_tokens, url_tokens_count); }

  return -1;
}
