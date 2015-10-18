#define _GNU_SOURCE

#include <search.h>
#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_table.h"
#include "../../infrastructure/hash/hash_values.h"
#include "../../infrastructure/mem/mem.h"

// allocates a hash table
hash_table_t *hash_table_malloc(int maximum_distinct_keys_count)
{
  hash_table_t *hash_table_return = NULL;
  hsearch_data *hsearch_return = NULL;

  hash_table_return = calloc(1, sizeof(hash_table_t));
  check_mem(hash_table_return);

  hsearch_return = calloc(1, sizeof(hsearch_data));
  check_mem(hsearch_return);

  check_result_not(hcreate_r(maximum_distinct_keys_count * 1.5, hsearch_return), 0);

  hash_table_return->hsearch = hsearch_return;

  hash_table_return->keys = NULL;
  hash_table_return->keys_allocated_count = 0;
  hash_table_return->keys_used_count = 0;

  hash_table_return->hash_values = NULL;
  hash_table_return->hash_values_allocated_count = 0;
  hash_table_return->hash_values_used_count = 0;

  goto cleanup;

error:

  if (hsearch_return != NULL)
  {
    hdestroy_r(hsearch_return);
    free(hsearch_return);
  }

  if (hash_table_return != NULL) { free(hash_table_return); }

  hash_table_return = NULL;

cleanup:

  return hash_table_return;
}

// gets the hash values for a given key
int hash_get_hash_values(hash_table_t *hash_table, char *key, hash_values_t **hash_values)
{
  int exit_code = 0;

  check_not_null(hash_table);
  check_not_null(key);
  check_not_null(hash_values);

  ENTRY find_entry;
  find_entry.key = key;

  ENTRY *existing_entry = NULL;

  int hsearch_find_result = hsearch_r(find_entry, FIND, &existing_entry, hash_table->hsearch);
  int hsearch_find_result_error = hsearch_find_result == 0 && errno != ESRCH;
  check_int(hsearch_find_result_error, 0);

  if (existing_entry != NULL)
  {
    *hash_values = (hash_values_t *)(existing_entry->data);
  }
  else
  {
    *hash_values = NULL;
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// gets or creates the hash values for a given key
int hash_get_or_create_hash_values(hash_table_t *hash_table, char *key, hash_values_t **hash_values)
{
  hash_values_t *hash_values_return = NULL;

  int exit_code = 0;

  char *key_copied = NULL;

  check_not_null(hash_table);
  check_not_null(key);
  check_not_null(hash_values);

  check_result(hash_get_hash_values(hash_table, key, &hash_values_return), 0);

  if (hash_values_return != NULL)
  {
    *hash_values = hash_values_return;

    goto cleanup;
  }

  check_result(malloc_memcpy_string(&key_copied, key), 0);

  hash_values_return = hash_values_malloc();
  check_not_null(hash_values_return);

  ENTRY new_entry;
  new_entry.key = key_copied;
  new_entry.data = hash_values_return;

  ENTRY *enter_entry = NULL;
  check_result_not(hsearch_r(new_entry, ENTER, &enter_entry, hash_table->hsearch), 0);
  check_not_null(enter_entry);

  check_result(
    array_add_string(
      &(hash_table->keys),
      &(hash_table->keys_allocated_count),
      &(hash_table->keys_used_count),
      key_copied),
    0);

  key_copied = NULL;

  check_result(
    array_add_pointer(
      (void ***)&(hash_table->hash_values),
      &(hash_table->hash_values_allocated_count),
      &(hash_table->hash_values_used_count),
      hash_values_return),
    0);

  *hash_values = hash_values_return;

  goto cleanup;

error:

  if (hash_values_return != NULL) { hash_values_free(hash_values_return); }

  exit_code = -1;

cleanup:

  if (key_copied != NULL) { free(key_copied); }

  return exit_code;
}

// adds a value for a given key
int hash_table_add_int_int(hash_table_t *hash_table, int key, int value)
{
  int exit_code = 0;

  char *key_string = NULL;

  check_not_null(hash_table);

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  check_result_greater(sprintf(key_string, "%d", key), 0);

  hash_values_t *hash_values = NULL;
  check_result(hash_get_or_create_hash_values(hash_table, key_string, &hash_values), 0);

  check_result(
    array_add_int(
      (int **)&(hash_values->values),
      &(hash_values->allocated_count),
      &(hash_values->used_count),
      value),
    0);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (key_string != NULL) { free(key_string); }

  return exit_code;
}

// adds a value for a given key
int hash_table_add_int_pointer(hash_table_t *hash_table, int key, void *value)
{
  int exit_code = 0;

  char *key_string = NULL;

  check_not_null(hash_table);

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  check_result_greater(sprintf(key_string, "%d", key), 0);

  hash_values_t *hash_values = NULL;
  check_result(hash_get_or_create_hash_values(hash_table, key_string, &hash_values), 0);

  check_result(
    array_add_pointer(
      (void ***)&(hash_values->values),
      &(hash_values->allocated_count),
      &(hash_values->used_count),
      value),
    0);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (key_string != NULL) { free(key_string); }

  return exit_code;
}

// gets the values for a given key
int hash_table_get_int_int(hash_table_t *hash_table, int key, int **values, int *values_count)
{
  int *values_return = NULL;
  int values_count_return = 0;

  int exit_code = 0;

  char *key_string = NULL;

  check_not_null(hash_table);
  check_not_null(values);
  check_not_null(values_count);

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  check_result_greater(sprintf(key_string, "%d", key), 0);

  hash_values_t *hash_values = NULL;
  check_result(hash_get_hash_values(hash_table, key_string, &hash_values), 0);

  if (hash_values != NULL)
  {
    values_return = (int *)(hash_values->values);
    values_count_return = hash_values->used_count;
  }
  else
  {
    values_return = NULL;
    values_count_return = 0;
  }

  *values = values_return;
  *values_count = values_count_return;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (key_string != NULL) { free(key_string); }

  return exit_code;
}

// gets the values for a given key
int hash_table_get_int_pointer(hash_table_t *hash_table, int key, void ***values, int *values_count)
{
  void **values_return = NULL;
  int values_count_return = 0;

  int exit_code = 0;

  char *key_string = NULL;

  check_not_null(hash_table);
  check_not_null(values);
  check_not_null(values_count);

  key_string = malloc(sizeof(char) * (sizeof(int) + 1));
  check_mem(key_string);

  check_result_greater(sprintf(key_string, "%d", key), 0);

  hash_values_t *hash_values = NULL;
  check_result(hash_get_hash_values(hash_table, key_string, &hash_values), 0);

  if (hash_values != NULL)
  {
    values_return = (void **)(hash_values->values);
    values_count_return = hash_values->used_count;
  }
  else
  {
    values_return = NULL;
    values_count_return = 0;
  }

  *values = values_return;
  *values_count = values_count_return;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (key_string != NULL) { free(key_string); }

  return exit_code;
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
