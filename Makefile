CC=gcc
CFLAGS=-fsanitize=address -Wall -Werror -std=gnu11 -g

tests: tests.c virtual_alloc.c
	$(CC) $(CFLAGS) $^ -o $@ -lm
run_tests:
	bash test.sh
clean: 
	rm -f tests
