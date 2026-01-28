# Christian Quintero
# COSC-4348 Systems Programming
# Assignment 1 part 2 - Makefiles
# 01/28/2026
#
# This program is a makefile for compiling all programs given in the assignment.

# target to run the executable COSC4348
# and build if needed
RunCOSC4348:
	./exec/COSC4348 

# target to build the executable COSC4348
# compiles main.cpp into main.o using the headers in ./headers
# compiles class.cpp into class.o using the headers in ./headers
# builds the executable in the exec directory using the object files made
COSC4348: 
	g++ -c ./cpp1/main.cpp -o ./cpp1/main.o -I./headers
	g++ -c ./cpps/class.cpp -o ./cpps/class.o -I./headers
	g++ -o ./exec/COSC4348 ./cpp1/main.o ./cpps/class.o

# target to clean up the object files and executable
CleanCOSC4348:
	rm -f ./cpp1/main.o ./cpps/class.o ./exec/COSC4348










