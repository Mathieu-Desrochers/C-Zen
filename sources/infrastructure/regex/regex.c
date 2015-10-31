#include <pcre.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/regex/regex.h"

// matches a string against a regular expression
int regex_match(char *pattern, char *string, int *matched, char ***tokens, int *tokens_count)
{
  char **tokens_return = NULL;

  int exit_code = 0;

  pcre *compiled_pcre = NULL;
  const char *error = NULL;
  int error_offset = 0;

  int *match_indexes = NULL;
  const char *match = NULL;
  char *match_copied = NULL;

  int tokens_allocated_count = 0;
  int tokens_used_count = 0;

  check_not_null(pattern);
  check_not_null(string);
  check_not_null(matched);
  check_not_null(tokens);
  check_not_null(tokens_count);

  compiled_pcre = pcre_compile(pattern, 0, &error, &error_offset, NULL);
  check(compiled_pcre != NULL, "compiled_pcre: NULL | pattern: %s | error: %s | error_offset: %d",
    pattern, error, error_offset);

  match_indexes = malloc(sizeof(int) * 30);
  check_mem(match_indexes);

  int pcre_exec_result = pcre_exec(compiled_pcre, NULL, string, strlen(string), 0, 0, match_indexes, 30);
  check((pcre_exec_result >= 1) || (pcre_exec_result == PCRE_ERROR_NOMATCH), "pcre_exec_result: %d",
    pcre_exec_result);

  if (pcre_exec_result == PCRE_ERROR_NOMATCH)
  {
    *matched = 0;
    goto cleanup;
  }

  for (int i = 1; i < pcre_exec_result; i++)
  {
    check_result_greater(pcre_get_substring(string, match_indexes, pcre_exec_result, i, &match), 0);

    check_result(malloc_memcpy_string(&match_copied, (char *)match), 0);

    check_result(
      array_add_string(
        &tokens_return,
        &tokens_allocated_count,
        &tokens_used_count,
        match_copied),
      0);

    match_copied = NULL;

    pcre_free_substring(match);
    match = NULL;
  }

  *matched = 1;
  *tokens = tokens_return;
  *tokens_count = pcre_exec_result - 1;

  goto cleanup;

error:

  if (tokens_return != NULL) { array_free_string(tokens_return, tokens_used_count); }

  exit_code = -1;

cleanup:

  if (match_copied != NULL) { free(match_copied); }
  if (match != NULL) { pcre_free_substring(match); }
  if (match_indexes != NULL) { free(match_indexes); }
  if (compiled_pcre != NULL) { pcre_free(compiled_pcre); }

  return exit_code;
}
