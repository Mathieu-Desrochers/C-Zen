CC = gcc
CFLAGS  = -g -Wall

TABLES = sources/core/tables/customer_row.o

all : $(TABLES)

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(TABLES)
