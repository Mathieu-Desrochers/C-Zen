#include <stdlib.h>
#include <time.h>

#include "../../core/tables/order_item_row.h"
#include "../../core/tables/order_items_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../infrastructure/time/time.h"

int main()
{
    order_item_row_t **order_item_rows = NULL;
    int count = 0;

    sqlite3 *sql_connection;
    int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
    check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

    int select_result = order_items_table_select_all(sql_connection, &order_item_rows, &count);
    check(select_result == 0, "select_result: %d",
      select_result);

    printf("%d\n", count);
    printf("%s\n", order_item_rows[0]->name);
    printf("%s\n", order_item_rows[1]->name);
    printf("%s\n", order_item_rows[2]->name);

    order_item_rows_free(order_item_rows, count);

    sql_close_connection(sql_connection);

    return 0;

error:

    if (order_item_rows != NULL) { order_item_rows_free(order_item_rows, count); }
    if (sql_connection != NULL) { sql_close_connection(sql_connection); }

    return -1;
}

