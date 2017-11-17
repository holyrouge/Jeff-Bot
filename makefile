make: test.c
	gcc test.c -o test.out

run: make
	./test.out
