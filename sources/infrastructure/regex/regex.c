#include <pcre.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/regex/regex.h"

// matches a string against a regular expression
int regex_match(char *pattern, char *string, int *matched, char ***tokens, int *tokens_count)
{
  const char *error = NULL;
  int error_offset = 0;
  int *match_indexes = NULL;
  const char *match = NULL;
  char *copied_match = NULL;

  char **tokens_return = NULL;
  int allocated_tokens_count = 0;
  int used_tokens_count = 0;

  pcre *compiled_pcre = pcre_compile(pattern, 0, &error, &error_offset, NULL);
  check(compiled_pcre != NULL, "compiled_pcre: NULL | error: %s | error_offset: %d",
    error, error_offset);

  match_indexes = malloc(sizeof(int) * 30);
  check_mem(match_indexes);

  int pcre_exec_result = pcre_exec(compiled_pcre, NULL, string, strlen(string), 0, 0, match_indexes, 30);
  check((pcre_exec_result >= 1) || (pcre_exec_result == PCRE_ERROR_NOMATCH), "pcre_exec_result: %d",
    pcre_exec_result);

  if (pcre_exec_result == PCRE_ERROR_NOMATCH)
  {
    free(match_indexes);
    pcre_free(compiled_pcre);

    *matched = 0;

    return 0;
  }

  for (int i = 1; i < pcre_exec_result; i++)
  {
    int pcre_get_substring_result = pcre_get_substring(string, match_indexes, pcre_exec_result, i, &match);
    check(pcre_get_substring_result >= 0, "pcre_get_substring_result: %d",
      pcre_get_substring_result);

    int malloc_memcpy_string_result = malloc_memcpy_string(&copied_match, (char *)match);
    check(malloc_memcpy_string_result == 0, "malloc_memcpy_string_result: %d",
      malloc_memcpy_string_result);

    int array_add_string_result = array_add_string(
      &tokens_return,
      &allocated_tokens_count,
      &used_tokens_count,
      copied_match);

    check(array_add_string_result == 0, "array_add_string_result: %d",
      array_add_string_result);

    copied_match = NULL;

    pcre_free_substring(match);

    match = NULL;
  }

  free(match_indexes);
  pcre_free(compiled_pcre);

  *matched = 1;
  *tokens = tokens_return;
  *tokens_count = pcre_exec_result - 1;

  return 0;

error:

  if (tokens_return != NULL)
  {
    for (int i = 0; i < used_tokens_count; i++)
    {
      free(tokens_return[i]);
    }

    free(tokens_return);
  }

  if (copied_match != NULL) { free(copied_match); }
  if (match != NULL) { pcre_free_substring(match); }
  if (match_indexes != NULL) { free(match_indexes); }
  if (compiled_pcre != NULL) { pcre_free(compiled_pcre); }

  return -1;
}
