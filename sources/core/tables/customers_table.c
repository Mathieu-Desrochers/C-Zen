#include <stddef.h>
#include <sqlite3.h>
#include <stddef.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

// selects a customer_row by id
customer_row_t *customers_table_select_by_customer_id(int customer_id)
{
  sqlite3 *sql_connection = NULL;

  int open_result = sqlite3_open("/var/c-zen/c-zen.db", &sql_connection);
  check(open_result == SQLITE_OK, "Could not open database.");

error:

  return NULL;
}
