#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../core/tables/customer_row.h"

// allocates a customer row
customer_row_t *customer_row_malloc(
  int customer_id,
  char *first_name,
  char *last_name,
  char *address,
  char *city,
  char *state,
  char *zip)
{
  customer_row_t *customer_row = malloc(sizeof(customer_row_t));
  check_mem(customer_row);

  customer_row->customer_id = customer_id;

  customer_row->first_name = malloc(strlen(first_name) + 1);
  check_mem(customer_row->first_name);
  strcpy(customer_row->first_name, first_name);

  customer_row->last_name = malloc(strlen(last_name) + 1);
  check_mem(customer_row->last_name);
  strcpy(customer_row->last_name, last_name);

  customer_row->address = malloc(strlen(address) + 1);
  check_mem(customer_row->address);
  strcpy(customer_row->address, address);

  customer_row->city = malloc(strlen(city) + 1);
  check_mem(customer_row->city);
  strcpy(customer_row->city, city);

  customer_row->state = malloc(strlen(state) + 1);
  check_mem(customer_row->state);
  strcpy(customer_row->state, state);

  customer_row->zip = malloc(strlen(zip) + 1);
  check_mem(customer_row->zip);
  strcpy(customer_row->zip, zip);

  return customer_row;

error:

  customer_row_free(customer_row);

  return NULL;
}

// frees a customer row
void customer_row_free(customer_row_t *customer_row)
{
  if (customer_row == NULL)
  {
    return;
  }

  if (customer_row->first_name != NULL) { free(customer_row->first_name); }
  if (customer_row->last_name != NULL) { free(customer_row->last_name); }
  if (customer_row->address != NULL) { free(customer_row->address); }
  if (customer_row->city != NULL) { free(customer_row->city); }
  if (customer_row->state != NULL) { free(customer_row->state); }
  if (customer_row->zip != NULL) { free(customer_row->zip); }

  free(customer_row);
}

// reallocates an array of customer rows
customer_row_t **customer_rows_realloc(customer_row_t **customer_rows, int count)
{
  customer_row_t **reallocated_customer_rows = realloc(customer_rows, sizeof(customer_row_t*) * count);
  check_mem(reallocated_customer_rows);

  return reallocated_customer_rows;

error:

  return NULL;
}

// frees an array of customer rows
void customer_rows_free(customer_row_t **customer_rows, int count)
{
  if (customer_rows == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    customer_row_free(customer_rows[i]);
  }

  free(customer_rows);
}
