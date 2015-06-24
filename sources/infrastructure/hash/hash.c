#define _GNU_SOURCE

#include <search.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash.h"

// allocates a hash table
int hash_malloc(hash_table_t **hash_table, int maximum_keys_count)
{
  hash_table_t *hash_table_result = NULL;
  hsearch_data *hsearch = NULL;
  char **keys = NULL;

  hash_table_result = malloc(sizeof(hash_table_t));
  check_mem(hash_table_result);

  hsearch = calloc(1, sizeof(hsearch_data));
  check_mem(hsearch);

  int hcreate_result = hcreate_r(maximum_keys_count * 1.5, hsearch);
  check(hcreate_result != 0, "hcreate_result: %d",
    hcreate_result);

  keys = malloc(sizeof(char *) * maximum_keys_count);
  check_mem(keys);

  hash_table_result->hsearch = hsearch;
  hash_table_result->keys = keys;
  hash_table_result->allocated_keys_count = maximum_keys_count;
  hash_table_result->used_keys_count = 0;

  *hash_table = hash_table_result;

  return 0;

error:

  if (keys != NULL) { free(keys); }
  if (hsearch != NULL) { free(hsearch); }
  if (hash_table_result != NULL) { free(hash_table_result); }

  return -1;
}

// sets the value for a given key
int hash_set(hash_table_t *hash_table, int key, void *value)
{
  char *key_string = NULL;
  ENTRY *existing_entry = NULL;
  ENTRY *new_entry = NULL;

  key_string = malloc(sizeof(char) * 16);
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  ENTRY entry;
  entry.key = key_string;
  entry.data = value;

  int hsearch_find_result = hsearch_r(entry, FIND, &existing_entry, hash_table->hsearch);
  check(hsearch_find_result != 0 || (hsearch_find_result == 0 && errno == ESRCH), "hsearch_find_result: %d",
    hsearch_find_result);

  if (existing_entry != NULL)
  {
    existing_entry->data = value;
    free(key_string);
    return 0;
  }

  check(hash_table->used_keys_count < hash_table->allocated_keys_count,
    "used_keys_count: %d | allocated_keys_count: %d",
    hash_table->used_keys_count,
    hash_table->allocated_keys_count);

  int hsearch_enter_result = hsearch_r(entry, ENTER, &new_entry, hash_table->hsearch);
  check(hsearch_enter_result != 0, "hsearch_enter_result: %d",
    hsearch_enter_result);

  hash_table->keys[hash_table->used_keys_count] = key_string;
  hash_table->used_keys_count++;

  return 0;

error:

  if (key_string != NULL) { free(key_string); }

  return -1;
}

// gets the value for a given key
int hash_get(hash_table_t *hash_table, int key, void **value)
{
  char *key_string = NULL;
  ENTRY *existing_entry = NULL;

  key_string = malloc(sizeof(char) * 16);
  check_mem(key_string);

  int sprintf_result = sprintf(key_string, "%d", key);
  check(sprintf_result > 0, "sprintf_result: %d",
    sprintf_result);

  ENTRY entry;
  entry.key = key_string;
  entry.data = NULL;

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

  for (int i = 0; i < hash_table->used_keys_count; i++)
  {
    free(hash_table->keys[i]);
  }

  free(hash_table->keys);
  free(hash_table);
}
