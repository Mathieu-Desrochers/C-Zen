#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

// selects a customer_row by id
customer_row_t *customers_table_select_by_customer_id(int customer_id)
{
  sqlite3 *sql_connection = sql_open_connection("/var/c-zen/c-zen.db");

  if (sql_connection != NULL)
  {
    sql_close_connection(sql_connection);
  }

  return NULL;
}
