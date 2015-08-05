CC = c99
CFLAGS  = -g -Wall

all : $(INFRASTRUCTURE) $(CORE) $(HTTP) main_core main_http tags

INFRASTRUCTURE = sources/infrastructure/array/array.o \
                 sources/infrastructure/fastcgi/fastcgi.o \
                 sources/infrastructure/hash/hash_table.o \
                 sources/infrastructure/hash/hash_values.o \
                 sources/infrastructure/json/json.o \
                 sources/infrastructure/mem/mem.o \
                 sources/infrastructure/regex/regex.o \
                 sources/infrastructure/sql/sql.o \
                 sources/infrastructure/time/time.o \
                 sources/infrastructure/validation/validation.o

CORE = sources/core/tables/order_row.o \
       sources/core/tables/orders_table.o \
       sources/core/tables/order_item_row.o \
       sources/core/tables/order_items_table.o \
       sources/core/services/new_order_request.o \
       sources/core/services/new_order_request_order_item.o \
       sources/core/services/new_order_response.o \
       sources/core/services/new_order_service.o \
       sources/core/services/update_order_request.o \
       sources/core/services/update_order_request_order_item.o \
       sources/core/services/update_order_response.o \
       sources/core/services/update_order_service.o

HTTP = sources/http/services/new_order_request_http.o \
       sources/http/services/new_order_request_order_item_http.o \
       sources/http/services/new_order_response_http.o \
       sources/http/services/new_order_service_http.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

main_core : $(INFRASTRUCTURE) $(CORE) $(HTTP) sources/core/main/main.c
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(CORE) $(HTTP) \
	sources/core/main/main.c -lsqlite3 -ljansson -lfcgi -lpcre -o main_core

main_http : $(INFRASTRUCTURE) $(CORE) $(HTTP) sources/http/main/main.c
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(CORE) $(HTTP) \
	sources/http/main/main.c -lsqlite3 -ljansson -lfcgi -lpcre -o main_http

libraries: fastcgi \
           jansson

fastcgi:
	tar -xzf libraries/fcgi-2.4.0.tar.gz -C /tmp
	cd /tmp/fcgi-2.4.0/libfcgi && sed '25 i #include <stdio.h>' fcgio.cpp > fcgio.cpp.tmp
	cd /tmp/fcgi-2.4.0/libfcgi && mv fcgio.cpp.tmp fcgio.cpp
	cd /tmp/fcgi-2.4.0 && ./configure
	$(MAKE) -C /tmp/fcgi-2.4.0
	$(MAKE) -C /tmp/fcgi-2.4.0 install

jansson:
	tar -xzf libraries/jansson-2.7.tar.gz -C /tmp
	cd /tmp/jansson-2.7 && ./configure
	$(MAKE) -C /tmp/jansson-2.7
	$(MAKE) -C /tmp/jansson-2.7 install

database:
	mkdir -p /var/c-zen
	sqlite3 /var/c-zen/c-zen.db < sources/database/create_database.sql
	sqlite3 /var/c-zen/c-zen.db "PRAGMA journal_mode=WAL;"
	chown mathieu /var/c-zen/
	chown mathieu /var/c-zen/c-zen.db

tags : $(INFRASTRUCTURE) $(CORE) $(HTTP)
	ctags -R .

clean :
	find . -name *.o | xargs -i /bin/rm {}
	rm -f main_core
	rm -f main_http
	rm -f tags
