all: stack.o enter.o tokenize.o reverse_polish_notation.o mini_calculator.o main.o
	gcc -o main stack.o enter.o tokenize.o reverse_polish_notation.o mini_calculator.o main.o

tests: stack.o enter.o tokenize.o reverse_polish_notation.o mini_calculator.o tests.o
	gcc -o tests stack.o enter.o tokenize.o reverse_polish_notation.o mini_calculator.o tests.o

stack.o: src/stack.c
	gcc -c src/stack.c

enter.o: src/enter.c
	gcc -c src/enter.c

tokenize.o: src/tokenize.c
	gcc -c src/tokenize.c

reverse_polish_notation.o: src/reverse_polish_notation.c
	gcc -c src/reverse_polish_notation.c

mini_calculator.o: src/mini_calculator.c
	gcc -c src/mini_calculator.c

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
