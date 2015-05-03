#include <stddef.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

// selects a customer_row by id
int customers_table_select_by_customer_id(sqlite3 *sql_connection, int customer_id, customer_row_t **customer_row)
{
  sqlite3_stmt *sql_statement = sql_prepare_statement(sql_connection, "SELECT * FROM customers WHERE \"customer-id\" = ?1;");
  check(sql_statement != NULL, "sql_statement == NULL");

  int sql_bind_int_result = sql_bind_int(sql_statement, 1, customer_id);
  check(sql_bind_int_result == 0, "sql_bind_int_result: %d", sql_bind_int_result);

  int sql_step_result = sql_step(sql_statement);
  check(sql_step_result == 0, "sql_step_result: %d", sql_step_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  sql_finalize_statement(sql_statement);

  return -1;
}
