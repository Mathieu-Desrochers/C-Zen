#ifndef JSON_H
#define JSON_H

#include <jansson.h>

// bookkeeping data structure for any memory
// allocated during the building of json objects
typedef struct json_context_t {
  char **strings;
  int strings_allocated_count;
  int strings_used_count;
} json_context_t;

// parses a json string
int json_parse_string(char *string, json_t **json);

// gets a json value from a key
int json_object_get_array(json_t *object, char *key, json_t **value, int *size);
int json_object_get_bool(json_t *object, char *key, int **value);
int json_object_get_double(json_t *object, char *key, double **value);
int json_object_get_object(json_t *object, char *key, json_t **value);
int json_object_get_int(json_t *object, char *key, int **value);
int json_object_get_string(json_t *object, char *key, char **value);

// gets a json value from an index
int json_array_get_array(json_t *array, int index, json_t **value, int *size);
int json_array_get_bool(json_t *array, int index, int **value);
int json_array_get_double(json_t *array, int index, double **value);
int json_array_get_int(json_t *array, int index, int **value);
int json_array_get_object(json_t *array, int index, json_t **value);
int json_array_get_string(json_t *array, int index, char **value);

// allocates a json context
json_context_t *json_context_malloc();

// allocates a json object
json_t *json_object_malloc();
json_t *json_array_malloc();

// sets a json value for a key
int json_object_set_array(json_t *object, char *key, json_t *value, json_context_t *context);
int json_object_set_bool(json_t *object, char *key, int *value, json_context_t *context);
int json_object_set_double(json_t *object, char *key, double *value, json_context_t *context);
int json_object_set_int(json_t *object, char *key, int *value, json_context_t *context);
int json_object_set_object(json_t *object, char *key, json_t *value, json_context_t *context);
int json_object_set_string(json_t *object, char *key, char *value, json_context_t *context);

// sets a json value for the next index
int json_array_add_array(json_t *array, json_t *value, json_context_t *context);
int json_array_add_bool(json_t *array, int *value, json_context_t *context);
int json_array_add_double(json_t *array, double *value, json_context_t *context);
int json_array_add_int(json_t *array, int *value, json_context_t *context);
int json_array_add_object(json_t *array, json_t *value, json_context_t *context);
int json_array_add_string(json_t *array, char *value, json_context_t *context);

// formats a json to string
int json_format_string(json_t *json, char **string);

// frees a json object
void json_free(json_t *json);

// frees a json context
void json_context_free(json_context_t *json_context);

#endif
