CC = c99
CFLAGS  = -g -Wall

all : $(INFRASTRUCTURE) $(TABLES) $(SERVICES) tags

INFRASTRUCTURE = sources/infrastructure/array/array.o \
                 sources/infrastructure/hash/hash_table.o \
                 sources/infrastructure/hash/hash_values.o \
                 sources/infrastructure/mem/mem.o \
                 sources/infrastructure/sql/sql.o \
                 sources/infrastructure/time/time.o \
                 sources/infrastructure/validation/validation.o

TABLES = sources/core/tables/order_row.o \
         sources/core/tables/orders_table.o \
         sources/core/tables/order_item_row.o \
         sources/core/tables/order_items_table.o \

SERVICES = sources/core/services/new_order_request.o \
           sources/core/services/new_order_request_order_item.o \
           sources/core/services/new_order_response.o \
           sources/core/services/new_order_service.o \
           sources/core/services/update_order_request.o \
           sources/core/services/update_order_request_order_item.o \
           sources/core/services/update_order_response.o \
           sources/core/services/update_order_service.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

main : all sources/core/main/main.c
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(TABLES) $(SERVICES) \
	-l sqlite3 sources/core/main/main.c -o $@

libraries: jansson

jansson:
	tar -xzf libraries/jansson-2.7.tar.gz -C /tmp
	cd /tmp/jansson-2.7 && ./configure
	$(MAKE) -C /tmp/jansson-2.7
	$(MAKE) -C /tmp/jansson-2.7 install

database :
	mkdir -p /var/c-zen
	sqlite3 /var/c-zen/c-zen.db < sources/database/create_database.sql
	sqlite3 /var/c-zen/c-zen.db "PRAGMA journal_mode=WAL;"
	chown mathieu /var/c-zen/
	chown mathieu /var/c-zen/c-zen.db

tags : $(INFRASTRUCTURE) $(TABLES) $(SERVICES)
	ctags -R .

clean :
	find . -name *.o | xargs -i /bin/rm {}
	rm -f main
	rm -f tags
