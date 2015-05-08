#ifndef CUSTOMER_ROW_H
#define CUSTOMER_ROW_H

// represents a customer row
typedef struct customer_row_t
{
  int customer_id;
  char *first_name;
  char *last_name;
  char *address;
  char *city;
  char *state;
  char *zip;
} customer_row_t;

// allocates a customer row
customer_row_t *customer_row_malloc(
  int customer_id,
  char *first_name,
  char *last_name,
  char *address,
  char *city,
  char *state,
  char *zip);

// frees a customer row
void customer_row_free(customer_row_t *customer_row);

// reallocates an array of customer rows
customer_row_t **customer_rows_realloc(customer_row_t **customer_rows, int count);

// frees an array of customer rows
void customer_rows_free(customer_row_t **customer_rows, int count);

#endif
