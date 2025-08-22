build: sudoku.c
	gcc -o sudoku sudoku.c -pthread

sudoku: clean build
	./sudoku test.txt

clean:
	rm -f sudoku