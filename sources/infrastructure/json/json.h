#ifndef JSON_H
#define JSON_H

#include <jansson.h>

// parses a json string
int json_parse_string(char *string, json_t **json);

// gets a json value from a key
int json_get_bool(json_t *object, char *key, int **value);
int json_get_double(json_t *object, char *key, double **value);
int json_get_int(json_t *object, char *key, int **value);
int json_get_string(json_t *object, char *key, char **value);

// frees a parsed json string
void json_parse_string_free(json_t *json);

#endif
