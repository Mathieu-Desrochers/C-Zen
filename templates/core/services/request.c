`#'include <stdlib.h>

`#'include "../../core/services/NAME_SINGLE_LOWER()_request.h"
`#'include "../../infrastructure/dbg/dbg.h"
`#'include "../../infrastructure/mem/mem.h"
`#'include "../../infrastructure/validation/validation.h"

// allocates a NAME_SINGLE_LOWER() request
NAME_SINGLE_LOWER()_request_t *NAME_SINGLE_LOWER()_request_malloc(
  char *name)
{
  NAME_SINGLE_LOWER()_request_t *NAME_SINGLE_LOWER()_request = malloc(sizeof(NAME_SINGLE_LOWER()_request_t));
  check_mem(NAME_SINGLE_LOWER()_request);

  int malloc_memcpy_name_result = malloc_memcpy_string(&(NAME_SINGLE_LOWER()_request->name), name);
  check(malloc_memcpy_name_result == 0, "malloc_memcpy_name_result: %d",
    malloc_memcpy_name_result);

  NAME_SINGLE_LOWER()_request->subrequests = NULL;
  NAME_SINGLE_LOWER()_request->subrequests_count = 0;

  return NAME_SINGLE_LOWER()_request;

error:

  if (NAME_SINGLE_LOWER()_request != NULL) { NAME_SINGLE_LOWER()_request_free(NAME_SINGLE_LOWER()_request); }

  return NULL;
}

// validates a NAME_SINGLE_LOWER() request
int NAME_SINGLE_LOWER()_request_validate(
  NAME_SINGLE_LOWER()_request_t *NAME_SINGLE_LOWER()_request,
  validation_error_t ***validation_errors,
  int *count)
{
  validation_error_t **validation_errors_return = NULL;

  int allocated_errors_count = 0;
  int used_errors_count = 0;

  check(NAME_SINGLE_LOWER()_request != NULL, "NAME_SINGLE_LOWER()_request: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(count != NULL, "count: NULL");

  int validate_name_result = validate_string(NAME_SINGLE_LOWER()_request->name, 1, 1, 100);
  if (validate_name_result != 0)
  {
    int validation_errors_add_result = validation_errors_add(
      &validation_errors_return, &allocated_errors_count, &used_errors_count,
      NAME_SINGLE_UPPER()_REQUEST_NAME, -1, validate_name_result);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }

  int validate_subrequests_result = validate_array(
    (void **)(NAME_SINGLE_LOWER()_request->subrequests),
    NAME_SINGLE_LOWER()_request->subrequests_count,
    1, 1, 100);

  if (validate_subrequests_result != 0)
  {
    int validation_errors_add_result = validation_errors_add(
      &validation_errors_return, &allocated_errors_count, &used_errors_count,
      NAME_SINGLE_UPPER()_REQUEST_SUBREQUESTS, -1, validate_subrequests_result);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }
  else
  {
    if (NAME_SINGLE_LOWER()_request->subrequests != NULL)
    {
      for (int index = 0; index < NAME_SINGLE_LOWER()_request->subrequests_count; index++)
      {
        int NAME_SINGLE_LOWER()_request_subrequest_validate_result = NAME_SINGLE_LOWER()_request_subrequest_validate(
          NAME_SINGLE_LOWER()_request->subrequests[index],
          index,
          &validation_errors_return,
          &allocated_errors_count,
          &used_errors_count);

        check(NAME_SINGLE_LOWER()_request_subrequest_validate_result == 0, "NAME_SINGLE_LOWER()_request_subrequest_validate_result: %d",
          NAME_SINGLE_LOWER()_request_subrequest_validate_result);
      }
    }
  }

  *validation_errors = validation_errors_return;
  *count = used_errors_count;

  return 0;

error:

  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, used_errors_count); }

  return -1;
}

// frees a NAME_SINGLE_LOWER() request
void NAME_SINGLE_LOWER()_request_free(NAME_SINGLE_LOWER()_request_t *NAME_SINGLE_LOWER()_request)
{
  if (NAME_SINGLE_LOWER()_request == NULL)
  {
    return;
  }

  if (NAME_SINGLE_LOWER()_request->name != NULL) { free(NAME_SINGLE_LOWER()_request->name); }

  if (NAME_SINGLE_LOWER()_request->subrequests != NULL)
  {
    NAME_SINGLE_LOWER()_request_subrequests_free(NAME_SINGLE_LOWER()_request->subrequests, NAME_SINGLE_LOWER()_request->subrequests_count);
  }

  free(NAME_SINGLE_LOWER()_request);
}
