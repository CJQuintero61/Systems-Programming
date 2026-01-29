# Christian Quintero
# COSC-4348 Systems Programming
# Assignment 1 part 2 - Makefiles
# 01/29/2026
#
# This program is a makefile for compiling all programs given in the assignment.
#
# To run: 
#	1. Must be in a unix terminal like WSL/Linux/MacOS
# 	2. Navigate to ./HW01/problem2 
# 	3. Run a build target such as "make COSC4348"
#	4. Run a run target such as "make RunCOSC4348"
#	5. Run a clean target such as "make CleanCOSC4348"

# target to run the executable COSC4348
# will fail if the executable has not been built yet
RunCOSC4348:
	./exec/COSC4348 

# target to build the executable COSC4348
# will compile the necessary object files and place them
# in the /cpp1 directory and the executable in the /exec directory.
# since the header file is in a different directory, I used the -I flag
# to specify the path to the headers directory.
COSC4348: 
	g++ -c ./cpp1/main.cpp -o ./cpp1/main.o -I./headers
	g++ -c ./cpps/class.cpp -o ./cpp1/class.o -I./headers
	g++ -o ./exec/COSC4348 ./cpp1/main.o ./cpp1/class.o

# target to clean up the object files and executable
CleanCOSC4348:
	rm -f ./cpp1/main.o ./cpp1/class.o ./exec/COSC4348

# runs the executable COSC4354
RunCOSC4354:
	./exec/COSC4354

# builds the executable COSC4354
COSC4354:
	g++ -c ./cpp2/main.cpp -o ./cpp2/main.o -I./headers
	g++ -c ./cpps/class.cpp -o ./cpp2/class.o -I./headers
	g++ -o ./exec/COSC4354 ./cpp2/main.o ./cpp2/class.o

# clean the object files and executable for COSC4354
CleanCOSC4354:
	rm -f ./cpp2/main.o ./cpp2/class.o ./exec/COSC4354

# run COSC3346
RunCOSC3346:
	./exec/COSC3346

# build COSC3346
COSC3346:
	g++ -c ./cpp3/main.cpp -o ./cpp3/main.o -I./headers
	g++ -c ./cpps/class.cpp -o ./cpp3/class.o -I./headers
	g++ -o ./exec/COSC3346 ./cpp3/main.o ./cpp3/class.o

# clean COSC3346
CleanCOSC3346:
	rm -f ./cpp3/main.o ./cpp3/class.o ./exec/COSC3346

# clean all
# this can be done by just calling the individual clean targets
# already written above
# marked as phony to ignore default behavior
.PHONY: Cleanall
Cleanall:
	make CleanCOSC4348
	make CleanCOSC4354
	make CleanCOSC3346




