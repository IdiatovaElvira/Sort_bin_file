CC = gcc 
CFLAGS = -Wall -Wextra -Wall -std=c11

all: cipher

cipher: 
	$(CC) $(CFLAGS) state_sort.c struct.h -o state_sort


clean:
	rm -rf *.o state_sort

check-leaks: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./state_sort

format:
	clang-format -n state_sort.c

check: format check-leaks

rebuild: clean all