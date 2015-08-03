#include <stdio.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/regex/regex.h"

int main()
{
  int matched = 0;
  char **tokens = NULL;
  int tokens_count = 0;

  int regex_match_result = regex_match("A(\\d+)", "A123", &matched, &tokens, &tokens_count);
  check(regex_match_result == 0, "regex_match_result: %d",
    regex_match_result);

  printf("matched: %d\n", matched);

  for (int i = 0; i < tokens_count; i++)
  {
    printf("token %d: %s\n", i, tokens[i]);
  }

  for (int i = 0; i < tokens_count; i++)
  {
    free(tokens[i]);
  }

  free(tokens);

  return 0;

error:

  if (tokens != NULL)
  {
    for (int i = 0; i < tokens_count; i++)
    {
      free(tokens[i]);
    }

    free(tokens);
  }

  return -1;
}
