#ifndef JSON_H
#define JSON_H

#include <jansson.h>

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

// frees a parsed json string
void json_parse_string_free(json_t *json);

// allocates a json object
json_t *json_object_malloc();

// sets a json value for a key
int json_object_set_bool(json_t *json, char *key, int *value);
int json_object_set_double(json_t *json, char *key, double *value);
int json_object_set_int(json_t *json, char *key, int *value);
int json_object_set_object(json_t *json, char *key, json_t *value);
int json_object_set_string(json_t *json, char *key, char *value);

// writes a json to string
int json_to_string(json_t *json, char **string);

// frees a json object
void json_free(json_t *json);

#endif
