CC = gcc

all: original improved

improved : driver.o memory_management_improved.o
	$(CC) driver.o memory_management_improved.o -o improved

original : driver.o memory_management.o
	$(CC) driver.o memory_management.o -o original

driver.o : driver.c memory_management.h
	$(CC) -c driver.c -o driver.o

memory_management_improved.o : memory_management_improved.c memory_management.h
	$(CC) -c -g -W -Wall -Wextra -shared -fPIC memory_management_improved.c

memory_management.o : memory_management.c memory_management.h
	$(CC) -c -g -W -Wall -Wextra -shared -fPIC memory_management.c

clean cleanup:
	rm -f *.o improved original
