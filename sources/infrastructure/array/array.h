#ifndef ARRAY_H
#define ARRAY_H

// adds an element to an array
int array_add_int(int **array, int *allocated_count, int *used_count, int value);
int array_add_pointer(void ***array, int *allocated_count, int *used_count, void *value);
int array_add_string(char ***array, int *allocated_count, int *used_count, char *value);

// finds duplicates within an array
int array_find_duplicates_int(int *array, int array_count, int **duplicate_indexes, int *duplicate_indexes_count);

#endif

