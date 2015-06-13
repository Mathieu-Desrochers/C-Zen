#ifndef VALIDATION_H
#define VALIDATION_H

#include <time.h>

#define VALIDATION_RESULT_REQUIRED 1
#define VALIDATION_RESULT_TOO_LOW 2
#define VALIDATION_RESULT_TOO_HIGH 3
#define VALIDATION_RESULT_TOO_SHORT 4
#define VALIDATION_RESULT_TOO_LONG 5

// represents a validated property
typedef struct validation_path_t
{
  int property;
  int index;
  struct validation_path_t *next;
} validation_path_t;

// represents a validation error
typedef struct validation_error_t
{
  validation_path_t *validation_path;
  int error_code;
} validation_error_t;

// allocates a validation path
validation_path_t *validation_path_malloc(int property, int index, validation_path_t *next);

// allocates a validation error
validation_error_t *validation_error_malloc(int property, int index, int error_code);

// adds a validation error to an array
int validation_errors_add(validation_error_t ***validation_errors, int *count, int *used, int property, int index, int error_code);

// validates a double
int validate_double(double *value, int required, double min_value, double max_value);

// validates an integer
int validate_int(int *value, int required, int min_value, int max_value);

// validates a string
int validate_string(char *value, int required, int min_length, int max_length);

// validates a time
int validate_time(time_t *value, int required, time_t min_value, time_t max_value);

// frees a validation path
void validation_path_free(validation_path_t *validation_path);

// frees a validation error
void validation_error_free(validation_error_t *validation_error);

// frees an array of validation errors
void validation_errors_free(validation_error_t **validation_errors, int count);

#endif
