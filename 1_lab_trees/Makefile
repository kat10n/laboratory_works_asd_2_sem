all: tree.o cli.o main.o tests.o
	gcc -o main tree.o cli.o main.o tests.o

tree.o: src/tree.c
	gcc -c src/tree.c

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


