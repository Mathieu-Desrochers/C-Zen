#include <fcgiapp.h>
#include <stdio.h>
#include <stdlib.h>

#include "../infrastructure/dbg/dbg.h"
#include "../http/services/new_order_service_http.h"

int main()
{
  FCGX_Request* request = malloc(sizeof(FCGX_Request));
  check_mem(request);

  int init_result = FCGX_Init();
  check(init_result == 0, "init_result: %d",
    init_result);

  int init_request_result = FCGX_InitRequest(request, 0, 0);
  check(init_request_result == 0, "init_request_result: %d",
    init_request_result);

  while (FCGX_Accept_r(request) == 0)
  {
    FCGX_ParamArray envp = request->envp;

    char *method = FCGX_GetParam("REQUEST_METHOD", envp);
    char *url = FCGX_GetParam("REQUEST_URI", envp);

    log_info("%s", method);
    log_info("%s", url);

    int matched = 0;
    char **url_tokens = NULL;
    int url_tokens_count = 0;

    int parse_url_result = new_order_service_http_parse_url(method, url, &matched, &url_tokens, &url_tokens_count);
    check(parse_url_result == 0, "parse_url_result: %d",
      parse_url_result);

    FCGX_PutS("Content-type: text/html\r\n", request->out);
    FCGX_PutS("\r\n", request->out);
    FCGX_PutS("HELLO\r\n", request->out);

    if (matched == 1)
    {
      FCGX_PutS("HELLO!\r\n", request->out);
    }

    FCGX_Finish_r(request);
  }

  FCGX_Free(request, 1);
  free(request);

  return 0;

error:

  return -1;
}
