CC = gcc
CFLAGS  = -g -Wall

TABLES = sources/core/tables/customer_row.o \
         sources/core/tables/customers_table.o

all : $(TABLES)

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

main : sources/core/main/main.c $(TABLES)
	$(CC) $(CFLAGS) $(TABLES) -l sqlite3 $< -o $@

database :
	mkdir -p /var/c-zen
	sqlite3 /var/c-zen/c-zen.db < sources/database/create_database.sql
	sqlite3 /var/c-zen/c-zen.db "PRAGMA journal_mode=WAL;"
	chown mathieu /var/c-zen/
	chown mathieu /var/c-zen/c-zen.db

clean :
	rm -f $(TABLES)
	rm -f main
