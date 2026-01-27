.PHONY: all
all: main


main: main.o lib.o	
	gcc -o $@ $^

# % is a wildcard
# here we use it to make object files for all
# files that have the .o extension with their .c file depenedency
# then we use $< to refer to the .c file and $@ to refer to the target file
%.o: %.c
	gcc -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o main