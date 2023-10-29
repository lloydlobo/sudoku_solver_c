# Program / executable name.
PROGN := sudoku

# Flags for the build target.
#	-g Compiler flags for debugging.
CFLAGS := -g

build:
	clang -o $(PROGN) $(CFLAGS) main.c

clean:
	rm -f $(PROGN) *.o

# Additional flag for testing.
#	-DVERBOSE_FLAG for 'test' target build to enable verbose output.
test: CFLAGS += -DVERBOSE_FLAG

test: build
	-(./$(PROGN) tests/input01; \
		./$(PROGN) tests/input02; \
		./$(PROGN) tests/input03; \
		./$(PROGN) tests/input04; \
		./$(PROGN) tests/input05)
