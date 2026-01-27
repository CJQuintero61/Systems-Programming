.PHONY: all
all: main

# $@ is used to represent the target name
# here it represents main
# $^ is used to represent all dependencies
# here it represents main.o lib.o
main: main.o lib.o	
	gcc -o $@ $^

# $@ represents main.o
# $< represents the first dependency, which is main.c
main.o: main.c		
	gcc -c $< -o $@

# $@ represents lib.o
# $< represents lib.c
lib.o: lib.c
	gcc -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o main

# notice how $@ and $^ are used to simplify the commands.
# this then allows us to further simplyfy the Makefile using pattern rules
# and wildcards in the next version