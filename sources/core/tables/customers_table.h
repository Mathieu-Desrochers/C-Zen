#ifndef CUSTOMERS_TABLE_H
#define CUSTOMERS_TABLE_H

#include "../../core/tables/customer_row.h"

// selects a customer_row by customer id
customer_row_t *customers_table_select_by_customer_id(int customer_id);

#endif
