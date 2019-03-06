compileAndRun:
	gcc main.c -c
	gcc LINT.c -c 
	gcc main.o LINT.o
	./a.out
