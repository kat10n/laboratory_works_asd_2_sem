all: bubble_sort.o binary_search.o read_file.o cli.o main.o
	gcc -o main bubble_sort.o binary_search.o read_file.o cli.o main.o

tests: bubble_sort.o binary_search.o tests.o
	gcc -o tests bubble_sort.o binary_search.o tests.o

bubble_sort.o: src/bubble_sort.c
	gcc -c src/bubble_sort.c

binary_search.o: src/binary_search.c
	gcc -c src/binary_search.c

read_file.o: src/read_file.c
	gcc -c src/read_file.c

cli.o: src/cli.c
	gcc -c src/cli.c

main.o: src/main.c
	gcc -c src/main.c

tests.o: src/tests.c
	gcc -c src/tests.c

run:
	./main.exe

run_tests:
	./tests.exe

clean:
	del *.o *.exe
