#include <check.h>

#include "../../../sources/infrastructure/sql/sql.h"
#include "../../../sources/core/tables/customer_row.h"
#include "../../../sources/core/tables/customers_table.h"

// inserts a customer row
START_TEST(customers_table_insert_test)
{
  sqlite3 *sql_connection = NULL;
  sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);

  customer_row_t *customer_row = customer_row_malloc(
    0,
    "Alice",
    "Alisson",
    "1 Alisson Street",
    "Alisson City",
    "Alisson State",
    "AAAAA");

  customers_table_insert(sql_connection, customer_row);

  ck_assert_int_eq(customer_row->customer_id, 1);

  customer_row_free(customer_row);
}
END_TEST

// creates the tests suite
Suite* customers_table_create_tests_suite(void)
{
  TCase* customers_table_tests = tcase_create("customers_table_tests");
  tcase_add_test(customers_table_tests, customers_table_insert_test);

  Suite* customers_table_tests_suite = suite_create("customers_table_tests_suite");
  suite_add_tcase(customers_table_tests_suite, customers_table_tests);

  return customers_table_tests_suite;
}
