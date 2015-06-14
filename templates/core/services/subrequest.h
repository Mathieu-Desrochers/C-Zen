`#'ifndef NAME_SINGLE_UPPER()_REQUEST_`'SUBNAME_SINGLE_UPPER()_H
`#'define NAME_SINGLE_UPPER()_REQUEST_`'SUBNAME_SINGLE_UPPER()_H

`#'include "../../infrastructure/validation/validation.h"

`#'define NAME_SINGLE_UPPER()_REQUEST_`'SUBNAME_SINGLE_UPPER()_NAME 1
`#'define NAME_SINGLE_UPPER()_REQUEST_`'SUBNAME_SINGLE_UPPER()_WEIGHT 2

// represents a NAME_SINGLE_LOWER() request SUBNAME_SINGLE_LOWER()
typedef struct NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_t
{
  char *name;
  double *weight;
} NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_t;

// allocates a NAME_SINGLE_LOWER() request SUBNAME_SINGLE_LOWER()
NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_t *NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_malloc(
  char *name,
  double *weight);

// validates a NAME_SINGLE_LOWER() request SUBNAME_SINGLE_LOWER()
int NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_validate(
  NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_t *NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER(),
  int index,
  validation_error_t ***validation_errors_return,
  int *allocated_errors_count,
  int *detected_errors_count);

// frees a NAME_SINGLE_LOWER() request SUBNAME_SINGLE_LOWER()
void NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_free(NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_t *NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER());

// frees an array of NAME_SINGLE_LOWER() request SUBNAME_SINGLE_LOWER()s
void NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()s_free(NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER()_t **NAME_SINGLE_LOWER()_request_`'SUBNAME_SINGLE_LOWER(), int count);

#endif
