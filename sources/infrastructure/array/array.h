#ifndef ARRAY_H
#define ARRAY_H

// adds an element to an array
int array_add_pointer(void ***array, int *allocated_count, int *used_count, void *value);
int array_add_string(char ***array, int *allocated_count, int *used_count, char *value);

#endif

