CC = c99
CFLAGS  = -g -Wall

INFRASTRUCTURE = sources/infrastructure/sql/sql.o

TABLES = sources/core/tables/customer_row.o \
         sources/core/tables/customers_table.o

TESTS = tests/core/tables/customers_table_tests.o

all : $(INFRASTRUCTURE) $(TABLES) $(TESTS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

main : sources/core/main/main.c $(INFRASTRUCTURE) $(TABLES)
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(TABLES) \
	-l sqlite3 sources/core/main/main.c -o $@

test-runner : tests/test-runner.c $(INFRASTRUCTURE) $(TABLES) $(TESTS)
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(TABLES) $(TESTS) \
	-l sqlite3 `pkg-config --cflags --libs check` tests/test-runner.c -o $@

database :
	mkdir -p /var/c-zen
	sqlite3 /var/c-zen/c-zen.db < sources/database/create_database.sql
	sqlite3 /var/c-zen/c-zen.db "PRAGMA journal_mode=WAL;"
	chown mathieu /var/c-zen/
	chown mathieu /var/c-zen/c-zen.db

clean :
	find . -name *.o | xargs -i /bin/rm {}
	rm -f main
