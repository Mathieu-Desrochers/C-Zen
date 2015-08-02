#include <fcgiapp.h>
#include <stdio.h>
#include <stdlib.h>

#include "../infrastructure/dbg/dbg.h"

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
    FCGX_PutS("Content-type: text/html\r\n", request->out);
    FCGX_PutS("\r\n", request->out);
    FCGX_PutS("HELLO\r\n", request->out);

    FCGX_Finish_r(request);
  }

  FCGX_Free(request, 1);
  free(request);

  return 0;

error:

  return -1;
}
