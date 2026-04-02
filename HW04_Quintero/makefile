# makefile for HW04_Quintero

# compile the main.c file and link it with the copy.o object file to create the executable
main: main.c copy.o
	gcc -o main main.c copy.o

# compile the copy.c file into an object file
copy.o: copy.c
	gcc -c copy.c

# remove object files and the executable
clean:
	rm -f *.o main