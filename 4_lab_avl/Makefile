all: avl_tree.o console_and_files.o main.o
	gcc -o avl_tree avl_tree.o console_and_files.o main.o

tests: avl_tree.o console_and_files.o tests.o
	gcc -o tests avl_tree.o console_and_files.o tests.o

avl_tree.o: src/avl_tree.c
	gcc -c src/avl_tree.c

console_and_files.o: src/console_and_files.c
	gcc -c src/console_and_files.c

main.o: src/main.c
	gcc -c src/main.c

tests.o: src/tests.c
	gcc -c src/tests.c

.PHONY: run run_tests clean

run:
	./avl_tree

run_tests:
	./tests

clean:
	rm -f *.o avl_tree tests