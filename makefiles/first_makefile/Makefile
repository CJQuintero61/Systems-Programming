# created by calling a file "Makefile" with no extension

# the target to be made
# in this case, the target is "all"
.PHONY: all
all: main

# compile object files into executable
main: main.o lib.o	# this line specifies what files are needed to create "main"
	gcc -o main main.o lib.o

# compile main.o by compiling main.c into an object file
main.o: main.c		# main.o depends on main.c
	gcc -c main.c -o main.o

# compile lib.o by compiling lib.c into an object file
lib.o: lib.c		# lib.o depends on lib.c
	gcc -c lib.c -o lib.o

# by running "make" the first target is built which is "all"
# This then builds "main" which in turn builds "main.o" and "lib.o" as needed
# so by running "make" we get the final executable "main"

# *.o means to remove all .o files
# -f forces the removal without prompting
.PHONY: clean
clean:
	rm -f *.o main

# .PHONY is used to run commands that are not files
# so running a phony target will always execute the commands