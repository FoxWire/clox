SOURCES_ALL = $(wildcard src/*.c) # all the .c files
SOURCES_NO_MAIN = $(filter-out src/main.c, $(SOURCES_ALL))
	# remove main.c for unit tests, minunit must already use main
OBJECTS_ALL = $(patsubst %.c,%.o,$(SOURCES_ALL)) # all the .o files

TEST_SOURCES = $(wildcard tests/*_tests.c)
TEST_TARGETS = $(patsubst %.c,%,$(TEST_SOURCES))

# Any changes in any header should also trigger recompilation. The rule says that .o files depend on their .c files and all headers.
HEADERS = $(wildcard src/*.h)
%.o: %.c $(HEADERS)
	$(CC) -g -Wall -Wextra -c -o  $@ $<

clox: $(OBJECTS_ALL)
	$(CC) -o bin/clox $(OBJECTS_ALL)

# Compiles each test target from its .c file. Also compiles in other source
# otherwise you can't access production code from the test code.
$(TEST_TARGETS): %: %.c $(SOURCES_NO_MAIN) $(HEADERS)
	$(CC) -o $@ $< $(SOURCES_NO_MAIN)

# 
.PHONY: test
test: $(TEST_TARGETS)
	sh ./tests/runtests.sh

.PHONY: clean
clean:
	rm -f tests/tests.log 
	rm -f tests/*_tests
	rm -f src/*.o
	rm -f bin/*

# Would still be nice to move the object files somewhere out of the way.





















