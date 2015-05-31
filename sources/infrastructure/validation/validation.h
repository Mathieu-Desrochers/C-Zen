#ifndef VALIDATION_H
#define VALIDATION_H

#define VALIDATION_RESULT_REQUIRED 1
#define VALIDATION_RESULT_TOO_SHORT 2
#define VALIDATION_RESULT_TOO_LONG 3

// represents the path to a validated property
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

// validates a string
int validate_string(char *value, int required, int min_length, int max_length);

// frees a validation path
void validation_path_free(validation_path_t *validation_path);

// frees a validation error
void validation_error_free(validation_error_t *validation_error);

#endif
