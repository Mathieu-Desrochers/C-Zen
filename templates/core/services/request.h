`#'ifndef NAME_SINGLE_UPPER()_REQUEST_H
`#'define NAME_SINGLE_UPPER()_REQUEST_H

`#'include "../../infrastructure/validation/validation.h"

`#'define NAME_SINGLE_UPPER()_REQUEST_NAME 1
`#'define NAME_SINGLE_UPPER()_REQUEST_WEIGHT 2
`#'define NAME_SINGLE_UPPER()_REQUEST_SUBREQUESTS 3

// represents a NAME_SINGLE_LOWER() request
typedef struct NAME_SINGLE_LOWER()_request_t
{
  char *name;
  NAME_SINGLE_LOWER()_request_subrequest_t **subrequests;
  int subrequests_count;
} NAME_SINGLE_LOWER()_request_t;

// allocates a NAME_SINGLE_LOWER() request
NAME_SINGLE_LOWER()_request_t *NAME_SINGLE_LOWER()_request_malloc(
  char *name);

// validates a NAME_SINGLE_LOWER() request
int NAME_SINGLE_LOWER()_request_validate(
  NAME_SINGLE_LOWER()_request_t *NAME_SINGLE_LOWER()_request,
  validation_error_t ***validation_errors,
  int *count);

// frees a NAME_SINGLE_LOWER() request
void NAME_SINGLE_LOWER()_request_free(NAME_SINGLE_LOWER()_request_t *NAME_SINGLE_LOWER()_request);

// frees an array of NAME_SINGLE_LOWER() requests
void NAME_SINGLE_LOWER()_requests_free(NAME_SINGLE_LOWER()_request_t **NAME_SINGLE_LOWER()_requests, int count);

#endif
