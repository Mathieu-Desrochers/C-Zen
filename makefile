CC = gcc
CFLAGS  = -std=c99 -g -Wall

all : $(INFRASTRUCTURE) $(CORE) $(HTTP) main-core main-http tags

INFRASTRUCTURE = sources/infrastructure/array/array.o \
                 sources/infrastructure/fastcgi/fastcgi.o \
                 sources/infrastructure/hash/hash_table.o \
                 sources/infrastructure/hash/hash_values.o \
                 sources/infrastructure/http/http.o \
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

main-core : $(INFRASTRUCTURE) $(CORE) $(HTTP) sources/core/main/main.c
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(CORE) $(HTTP) \
	sources/core/main/main.c -lsqlite3 -ljansson -lfcgi -lpcre -o main-core

main-http : $(INFRASTRUCTURE) $(CORE) $(HTTP) sources/http/main/main.c
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(CORE) $(HTTP) \
	sources/http/main/main.c -lsqlite3 -ljansson -lfcgi -lpcre -o main-http

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
	mkdir -p /var/main-http
	sqlite3 /var/main-http/database.db < sources/database/create_database.sql
	sqlite3 /var/main-http/database.db "PRAGMA journal_mode=WAL;"
	chown lighttpd:lighttpd /var/main-http
	chown lighttpd:lighttpd /var/main-http/database.db

http-configuration:
	echo '' >> /etc/lighttpd/lighttpd.conf
	echo 'fastcgi.server = (' >> /etc/lighttpd/lighttpd.conf
	echo '  "/api/" =>' >> /etc/lighttpd/lighttpd.conf
	echo '  ( "main-http" =>' >> /etc/lighttpd/lighttpd.conf
	echo '    (' >> /etc/lighttpd/lighttpd.conf
	echo '      "bin-path" => "/usr/local/bin/main-http-wrapper",' >> /etc/lighttpd/lighttpd.conf
	echo '      "check-local" => "disable",' >> /etc/lighttpd/lighttpd.conf
	echo '      "socket" => "/tmp/main-http.socket",' >> /etc/lighttpd/lighttpd.conf
	echo '      "strip-request-uri" => "/api/"' >> /etc/lighttpd/lighttpd.conf
	echo '    )' >> /etc/lighttpd/lighttpd.conf
	echo '  )' >> /etc/lighttpd/lighttpd.conf
	echo ')' >> /etc/lighttpd/lighttpd.conf
	touch /usr/local/bin/main-http-wrapper
	chmod +x /usr/local/bin/main-http-wrapper
	echo '#!/bin/bash' >> /usr/local/bin/main-http-wrapper
	echo 'export LD_LIBRARY_PATH=/usr/local/lib' >> /usr/local/bin/main-http-wrapper
	echo '/usr/local/bin/main-http 2>>/var/log/lighttpd/main-http.log' >> /usr/local/bin/main-http-wrapper
	touch /var/log/lighttpd/main-http.log
	chown lighttpd:lighttpd /var/log/lighttpd/main-http.log

http-install:
	/etc/rc.d/rc.lighttpd stop
	- pkill main-http
	cp main-http /usr/local/bin
	/etc/rc.d/rc.lighttpd start

tags : $(INFRASTRUCTURE) $(CORE) $(HTTP)
	ctags -R .

clean :
	find . -name *.o | xargs -i /bin/rm {}
	rm -f main-core
	rm -f main-http
	rm -f tags
