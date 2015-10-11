CC = gcc
CFLAGS  = -std=c99 -g -Wall

all : $(INFRASTRUCTURE) $(WEB_API) web_api_main tags

INFRASTRUCTURE = sources/infrastructure/array/array.o \
                 sources/infrastructure/fastcgi/fastcgi.o \
                 sources/infrastructure/hash/hash_table.o \
                 sources/infrastructure/hash/hash_values.o \
                 sources/infrastructure/http/http.o \
                 sources/infrastructure/json/json.o \
                 sources/infrastructure/mem/mem.o \
                 sources/infrastructure/regex/regex.o \
                 sources/infrastructure/sql/sql.o \
                 sources/infrastructure/string/string.o \
                 sources/infrastructure/time/time.o \
                 sources/infrastructure/validation/validation.o \
                 sources/infrastructure/validation_json/validation_json.o

WEB_API = sources/web_api/bindings/get_order_request_http.o \
          sources/web_api/bindings/get_order_response_http.o \
          sources/web_api/bindings/get_order_response_order_item_http.o \
          sources/web_api/bindings/get_order_service_http.o \
          sources/web_api/bindings/get_orders_request_http.o \
          sources/web_api/bindings/get_orders_response_http.o \
          sources/web_api/bindings/get_orders_response_order_http.o \
          sources/web_api/bindings/get_orders_service_http.o \
          sources/web_api/bindings/new_order_request_http.o \
          sources/web_api/bindings/new_order_request_order_item_http.o \
          sources/web_api/bindings/new_order_response_http.o \
          sources/web_api/bindings/new_order_service_http.o \
          sources/web_api/bindings/update_order_request_http.o \
          sources/web_api/bindings/update_order_request_order_item_http.o \
          sources/web_api/bindings/update_order_response_http.o \
          sources/web_api/bindings/update_order_service_http.o \
          sources/web_api/tables/order_row.o \
          sources/web_api/tables/orders_table.o \
          sources/web_api/tables/order_item_row.o \
          sources/web_api/tables/order_items_table.o \
          sources/web_api/services/get_order_request.o \
          sources/web_api/services/get_order_response.o \
          sources/web_api/services/get_order_response_order_item.o \
          sources/web_api/services/get_order_service.o \
          sources/web_api/services/get_orders_request.o \
          sources/web_api/services/get_orders_response.o \
          sources/web_api/services/get_orders_response_order.o \
          sources/web_api/services/get_orders_service.o \
          sources/web_api/services/new_order_request.o \
          sources/web_api/services/new_order_request_order_item.o \
          sources/web_api/services/new_order_response.o \
          sources/web_api/services/new_order_service.o \
          sources/web_api/services/update_order_request.o \
          sources/web_api/services/update_order_request_order_item.o \
          sources/web_api/services/update_order_response.o \
          sources/web_api/services/update_order_service.o

%.o : %.c
	m4 sources/infrastructure/m4/macros.m4 --synclines $< > $<~
	$(CC) $(CFLAGS) -xc -c $<~ -o $@
	@rm $<~

web_api_main: $(INFRASTRUCTURE) $(WEB_API) sources/web_api/main/main.c
	m4 sources/infrastructure/m4/macros.m4 --synclines sources/web_api/main/main.c > sources/web_api/main/main.c~
	$(CC) $(CFLAGS) $(INFRASTRUCTURE) $(WEB_API) -xc sources/web_api/main/main.c~ \
	-lsqlite3 -ljansson -lfcgi -lpcre -o web_api_main
	@rm sources/web_api/main/main.c~

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
	mkdir -p /var/web_api
	sqlite3 /var/web_api/database.db < sources/web_api/database/create_database.sql
	sqlite3 /var/web_api/database.db "PRAGMA journal_mode=WAL;"
	chown lighttpd:lighttpd /var/web_api
	chown lighttpd:lighttpd /var/web_api/database.db

http_configuration:
	echo '' >> /etc/lighttpd/lighttpd.conf
	echo 'fastcgi.server = (' >> /etc/lighttpd/lighttpd.conf
	echo '  "/api/" =>' >> /etc/lighttpd/lighttpd.conf
	echo '  ( "web_api_main" =>' >> /etc/lighttpd/lighttpd.conf
	echo '    (' >> /etc/lighttpd/lighttpd.conf
	echo '      "bin-path" => "/usr/local/bin/web_api_main_wrapper",' >> /etc/lighttpd/lighttpd.conf
	echo '      "check-local" => "disable",' >> /etc/lighttpd/lighttpd.conf
	echo '      "socket" => "/tmp/web_api_main.socket",' >> /etc/lighttpd/lighttpd.conf
	echo '      "strip-request-uri" => "/api/"' >> /etc/lighttpd/lighttpd.conf
	echo '    )' >> /etc/lighttpd/lighttpd.conf
	echo '  )' >> /etc/lighttpd/lighttpd.conf
	echo ')' >> /etc/lighttpd/lighttpd.conf
	touch /usr/local/bin/web_api_main_wrapper
	chmod +x /usr/local/bin/web_api_main_wrapper
	echo '#!/bin/bash' >> /usr/local/bin/web_api_main_wrapper
	echo 'export LD_LIBRARY_PATH=/usr/local/lib' >> /usr/local/bin/web_api_main_wrapper
	echo '/usr/local/bin/web_api_main 2>>/var/log/lighttpd/web_api_main.log' >> /usr/local/bin/web_api_main_wrapper
	touch /var/log/lighttpd/web_api_main.log
	chown lighttpd:lighttpd /var/log/lighttpd/web_api_main.log

http_run:
	/etc/rc.d/rc.lighttpd stop
	- pkill web_api_main
	cp web_api_main /usr/local/bin
	/etc/rc.d/rc.lighttpd start

tags : $(INFRASTRUCTURE) $(CORE) $(HTTP)
	ctags -R .

clean :
	find . -name *.o | xargs -i /bin/rm {}
	rm -f web_api_main
	rm -f tags
