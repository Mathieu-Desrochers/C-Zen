#define _GNU_SOURCE

#include <search.h>
#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_table.h"
#include "../../infrastructure/hash/hash_values.h"
#include "../../infrastructure/mem/mem.h"

// allocates a hash table
int hash_table_malloc(hash_table_t **hash_table, int maximum_distinct_keys_count)
{
  hash_table_t *hash_table_return = NULL;
  hsearch_data *hsearch = NULL;

  check(hash_table != NULL, "hash_table: NULL");

  hash_table_return = malloc(sizeof(hash_table_t));
  check_mem(hash_table_return);

  hsearch = calloc(1, sizeof(hsearch_data));
  check_mem(hsearch);

  int hcreate_result = hcreate_r(maximum_distinct_keys_count * 1.5, hsearch);
  check(hcreate_result != 0, "hcreate_result: %d",
    hcreate_result);

  hash_table_return->hsearch = hsearch;

  hash_table_return->keys = NULL;
  hash_table_return->keys_allocated_count = 0;
  hash_table_return->keys_used_count = 0;

  hash_table_return->hash_values = NULL;
  hash_table_return->hash_values_allocated_count = 0;
  hash_table_return->hash_values_used_count = 0;

  *hash_table = hash_table_return;

  return 0;

error:

  if (hsearch != NULL)
  {
    hdestroy_r(hsearch);
    free(hsearch);
  }

  if (hash_table_return != NULL) { free(hash_table_return); }

  return -1;
}

// gets the hash values for a given key
int hash_get_hash_values(hash_table_t *hash_table, char *key, hash_values_t **hash_values)
{
  check(hash_table != NULL, "hash_table: NULL");
  check(key != NULL, "key: NULL");
  check(hash_values != NULL, "hash_values: NULL");

  ENTRY find_entry;
  find_entry.key = key;

  ENTRY *existing_entry = NULL;

  int hsearch_find_result = hsearch_r(find_entry, FIND, &existing_entry, hash_table->hsearch);
  check(hsearch_find_result != 0 || (hsearch_find_result == 0 && errno == ESRCH), "hsearch_find_result: %d",
    hsearch_find_result);

  if (existing_entry == NULL)
  {
    *hash_values = NULL;
  }
  else
  {
    *hash_values = (hash_values_t *)(existing_entry->data);
  }

  return 0;

error:

  return -1;
}

// gets or creates the hash values for a given key
int hash_get_or_create_hash_values(hash_table_t *hash_table, char *key, hash_values_t **hash_values)
{
  hash_values_t *hash_values_return = NULL;
  char *key_copied = NULL;

  check(hash_table != NULL, "hash_table: NULL");
  check(key != NULL, "key: NULL");
  check(hash_values != NULL, "hash_values: NULL");

  int hash_get_hash_values_result = hash_get_hash_values(hash_table, key, &hash_values_return);
  check(hash_get_hash_values_result == 0, "hash_get_hash_values_result: %d",
    hash_get_hash_values_result);

  if (hash_values_return != NULL)
  {
    *hash_values = hash_values_return;

    return 0;
  }

  int malloc_memcpy_result = malloc_memcpy_string(&key_copied, key);
  check(malloc_memcpy_result == 0, "malloc_memcpy_result: %d",
    malloc_memcpy_result);

  int hash_values_malloc_result = hash_values_malloc(&hash_values_return);
  check(hash_values_malloc_result == 0, "hash_values_malloc_result: %d",
    hash_values_malloc_result);

  ENTRY new_entry;
  new_entry.key = key_copied;
  new_entry.data = hash_values_return;

  ENTRY *enter_entry = NULL;

  int hsearch_enter_result = hsearch_r(new_entry, ENTER, &enter_entry, hash_table->hsearch);
  check(hsearch_enter_result != 0, "hsearch_enter_result: %d",
    hsearch_enter_result);

  check(enter_entry != NULL, "enter_entry: NULL");

  int array_add_key_result = array_add_string(
    &(hash_table->keys),
    &(hash_table->keys_allocated_count),
    &(hash_table->keys_used_count),
    key_copied);

  check(array_add_key_result == 0, "array_add_key_result: %d",
    array_add_key_result);

  key_copied = NULL;

  int array_add_values_result = array_add_pointer(
    (void ***)&(hash_table->hash_values),
    &(hash_table->hash_values_allocated_count),
    &(hash_table->hash_values_used_count),
    hash_values_return);

  check(array_add_values_result == 0, "array_add_values_result: %d",
    array_add_values_result);

  hash_values_return = NULL;

  *hash_values = hash_values_return;

  return 0;

error:

  if (key_copied != NULL) { free(key_copied); }
  if (hash_values_return != NULL) { hash_values_free(hash_values_return); }

  return -1;
}

// adds a value for a given key
int hash_table_add_int_int(hash_table_t *hash_table, int key, int value)
{
  char *key_string = NULL;
  hash_values_t *hash_values = NULL;

  check(hash_table != NULL, "hash_table: NULL");

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  int hash_get_hash_values_result = hash_get_or_create_hash_values(hash_table, key_string, &hash_values);
  check(hash_get_hash_values_result == 0, "hash_get_hash_values_result: %d",
    hash_get_hash_values_result);

  int array_add_value_result = array_add_int(
    (int **)&(hash_values->values),
    &(hash_values->allocated_count),
    &(hash_values->used_count),
    value);

  check(array_add_value_result == 0, "array_add_value_result: %d",
    array_add_value_result);

  free(key_string);

  return 0;

error:

  if (key_string != NULL) { free(key_string); }

  return -1;
}

// adds a value for a given key
int hash_table_add_int_pointer(hash_table_t *hash_table, int key, void *value)
{
  char *key_string = NULL;
  hash_values_t *hash_values = NULL;

  check(hash_table != NULL, "hash_table: NULL");

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  int hash_get_hash_values_result = hash_get_or_create_hash_values(hash_table, key_string, &hash_values);
  check(hash_get_hash_values_result == 0, "hash_get_hash_values_result: %d",
    hash_get_hash_values_result);

  int array_add_value_result = array_add_pointer(
    (void ***)&(hash_values->values),
    &(hash_values->allocated_count),
    &(hash_values->used_count),
    value);

  check(array_add_value_result == 0, "array_add_value_result: %d",
    array_add_value_result);

  free(key_string);

  return 0;

error:

  if (key_string != NULL) { free(key_string); }

  return -1;
}

// gets the values for a given key
int hash_table_get_int_int(hash_table_t *hash_table, int key, int **values, int *values_count)
{
  char *key_string = NULL;
  hash_values_t *hash_values = NULL;

  check(hash_table != NULL, "hash_table: NULL");
  check(values != NULL, "values: NULL");
  check(values_count != NULL, "values_count: NULL");

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  int hash_get_hash_values_result = hash_get_hash_values(hash_table, key_string, &hash_values);
  check(hash_get_hash_values_result == 0, "hash_get_hash_values_result: %d",
    hash_get_hash_values_result);

  if (hash_values == NULL)
  {
    *values = NULL;
    *values_count = 0;
  }
  else
  {
    *values = (int *)(hash_values->values);
    *values_count = hash_values->used_count;
  }

  free(key_string);

  return 0;

error:

  if (key_string != NULL) { free(key_string); }

  return -1;
}

// gets the values for a given key
int hash_table_get_int_pointer(hash_table_t *hash_table, int key, void ***values, int *values_count)
{
  char *key_string = NULL;
  hash_values_t *hash_values = NULL;

  check(hash_table != NULL, "hash_table: NULL");
  check(values != NULL, "values: NULL");
  check(values_count != NULL, "values_count: NULL");

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  int hash_get_hash_values_result = hash_get_hash_values(hash_table, key_string, &hash_values);
  check(hash_get_hash_values_result == 0, "hash_get_hash_values_result: %d",
    hash_get_hash_values_result);

  if (hash_values == NULL)
  {
    *values = NULL;
    *values_count = 0;
  }
  else
  {
    *values = (void **)(hash_values->values);
    *values_count = hash_values->used_count;
  }

  free(key_string);

  return 0;

error:

  if (key_string != NULL) { free(key_string); }

  return -1;
}

// frees a hash table
void hash_table_free(hash_table_t *hash_table)
{
  if (hash_table == NULL)
  {
    return;
  }

  hdestroy_r(hash_table->hsearch);
  free(hash_table->hsearch);

  for (int i = 0; i < hash_table->keys_used_count; i++)
  {
    free(hash_table->keys[i]);
  }

  for (int i = 0; i < hash_table->hash_values_used_count; i++)
  {
    hash_values_free(hash_table->hash_values[i]);
  }

  free(hash_table->keys);
  free(hash_table->hash_values);
  free(hash_table);
}
