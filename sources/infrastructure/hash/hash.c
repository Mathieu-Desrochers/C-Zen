#define _GNU_SOURCE

#include <search.h>
#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash.h"
#include "../../infrastructure/mem/mem.h"

// allocates a hash table
int hash_malloc(hash_table_t **hash_table, int maximum_keys_count)
{
  hash_table_t *hash_table_return = NULL;
  hsearch_data *hsearch = NULL;

  hash_table_return = malloc(sizeof(hash_table_t));
  check_mem(hash_table_return);

  hsearch = calloc(1, sizeof(hsearch_data));
  check_mem(hsearch);

  int hcreate_result = hcreate_r(maximum_keys_count * 1.5, hsearch);
  check(hcreate_result != 0, "hcreate_result: %d",
    hcreate_result);

  hash_table_return->hsearch = hsearch;

  hash_table_return->keys = NULL;
  hash_table_return->keys_allocated_count = 0;
  hash_table_return->keys_used_count = 0;

  hash_table_return->values = NULL;
  hash_table_return->values_allocated_count = 0;
  hash_table_return->values_used_count = 0;

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

// sets the value for a given key
int hash_set(hash_table_t *hash_table, char *key, void *value)
{
  ENTRY *existing_entry = NULL;
  ENTRY *new_entry = NULL;

  check(hash_table != NULL, "hash_table: NULL");
  check(key != NULL, "key: NULL");

  ENTRY entry;
  entry.key = key;

  int hsearch_find_result = hsearch_r(entry, FIND, &existing_entry, hash_table->hsearch);
  check(hsearch_find_result != 0 || (hsearch_find_result == 0 && errno == ESRCH), "hsearch_find_result: %d",
    hsearch_find_result);

  if (existing_entry != NULL)
  {
    existing_entry->data = value;
    return 0;
  }

  entry.data = value;

  int hsearch_enter_result = hsearch_r(entry, ENTER, &new_entry, hash_table->hsearch);
  check(hsearch_enter_result != 0, "hsearch_enter_result: %d",
    hsearch_enter_result);

  return 0;

error:

  return -1;
}

// sets the value for a given key
int hash_set_int_int(hash_table_t *hash_table, int key, int value)
{
  char *key_string = NULL;
  int *value_copied = NULL;

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  int malloc_memcpy_result = malloc_memcpy_int(&value_copied, &value);
  check(malloc_memcpy_result == 0, "malloc_memcpy_result: %d",
    malloc_memcpy_result);

  int hash_set_result = hash_set(hash_table, key_string, value_copied);
  check(hash_set_result == 0, "hash_set_result: %d",
    hash_set_result);

  int array_add_key_result = array_add_string(
    &(hash_table->keys),
    &(hash_table->keys_allocated_count),
    &(hash_table->keys_used_count),
    key_string);

  check(array_add_key_result == 0, "array_add_key_result: %d",
    array_add_key_result);

  key_string = NULL;

  int array_add_value_result = array_add_pointer(
    &(hash_table->values),
    &(hash_table->values_allocated_count),
    &(hash_table->values_used_count),
    value_copied);

  check(array_add_value_result == 0, "array_add_value_result: %d",
    array_add_value_result);

  value_copied = NULL;

  return 0;

error:

  if (value_copied != NULL) { free(value_copied); }
  if (key_string != NULL) { free(key_string); }

  return -1;
}

// gets the value for a given key
int hash_get(hash_table_t *hash_table, char *key, void **value)
{
  ENTRY *existing_entry = NULL;

  ENTRY entry;
  entry.key = key;

  int hsearch_find_result = hsearch_r(entry, FIND, &existing_entry, hash_table->hsearch);
  check(hsearch_find_result != 0 || (hsearch_find_result == 0 && errno == ESRCH), "hsearch_find_result: %d",
    hsearch_find_result);

  if (existing_entry != NULL)
  {
    *value = existing_entry->data;
  }
  else
  {
    *value = NULL;
  }

  return 0;

error:

  return -1;
}

// gets the value for a given key
int hash_get_int_int(hash_table_t *hash_table, int key, int *value)
{
  char *key_string = NULL;
  int *value_return = NULL;

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  int hash_get_result = hash_get(hash_table, key_string, (void **)&value_return);
  check(hash_get_result == 0, "hash_get_result: %d",
    hash_get_result);

  if (value_return == NULL)
  {
    free(key_string);
    return 1;
  }

  *value = *value_return;

  free(key_string);
  return 0;

error:

  if (key_string != NULL) { free(key_string); }

  return -1;
}

// frees a hash table
void hash_free(hash_table_t *hash_table)
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

  for (int i = 0; i < hash_table->values_used_count; i++)
  {
    free(hash_table->values[i]);
  }

  free(hash_table->keys);
  free(hash_table->values);
  free(hash_table);
}
