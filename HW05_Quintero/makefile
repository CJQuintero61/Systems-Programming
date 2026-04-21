#   Christian Quintero
#   COSC 4348 - Systems Programming
#   04/13/2026
#
#   makefile
#	this makefile compiles all the code together

# make all the executables
.PHONY all: program1 program2 program3

# make the object file for the signals library
signals.o: signals.c signals.h
	gcc -c signals.c -o signals.o

# compile the object files by linking the signals library
program1.o: program1.c signals.h
	gcc -c program1.c -o program1.o

program2.o: program2.c signals.h
	gcc -c program2.c -o program2.o

program3.o: program3.c signals.h
	gcc -c program3.c -o program3.o

# create the executables with thier object files and thie signals object file
program1: program1.o signals.o
	gcc program1.o signals.o -o program1

program2: program2.o signals.o
	gcc program2.o signals.o -o program2

program3: program3.o signals.o
	gcc program3.o signals.o -o program3

.PHONY clean:
	rm -f *.o program1 program2 program3 P*.txt output.txt