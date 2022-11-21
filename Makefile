#CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
#LIBS=-ldl $(OPTLIBS)
#PREFIX?=/usr/local

#SOURCES=$(wildcard src/**/*.c src/*.c)
#OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

#TEST_SRC=$(wildcard tests/*_tests.c)
#TESTS=$(patsubst %.c,%,$(TEST_SRC))

#TARGET=build/clox.a
#SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The Target Build
#all: $(TARGET) $(SO_TARGET) build_clox tests

#dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
#dev: all

#$(TARGET): CFLAGS += -fPIC
#$(TARGET): build $(OBJECTS)
	#ar rcs $@ $(OBJECTS)
	#ranlib $@

#$(SO_TARGET): $(TARGET) $(OBJECTS)
	#$(CC) -shared -o $@ $(OBJECTS)

#build:
	#@mkdir -p build
	#@mkdir -p bin

# The Unit Tests
#.PHONY: tests
# tests: CFLAGS += $(TARGET)
# Had some problems linking here. Had to do what is described here:
# 	https://stackoverflow.com/questions/23851098/c-compile-errors-learn-c-the-hard-way-ex-32
#tests: LDLIBS += $(TARGET)
#tests: $(TESTS)
	#sh ./tests/runtests.sh

# The Cleaner
#clean:
	#rm -rf build $(OBJECTS) $(TESTS)
	#rm -f tests/tests.log 
	#find . -name "*.gc*" -exec rm {} \;
	#rm -rf `find . -name "*.dSYM" -print`

# The Install
#install: all
	#install -d $(DESTDIR)/$(PREFIX)/lib/
	#install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
#check:
	#@echo Files with potentially dangerous functions.
	#@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true

#Build main
#build_clox:
	#cc -o bin/clox src/*.c

# The runner
#clox:
	#./bin/clox



# NEW MAKE FILE
# create a clox binary with the main file
SOURCES=$(wildcard src/*.c) # all the .c files
OBJECTS=$(patsubst %.c,%.o,$(SOURCES)) # all the .o files

TEST_SOURCES=$(wildcard tests/_tests*.c)
TEST_OBJECTS=$(patsubst %.c,%,$(TEST_SOURCES))

clox: $(OBJECTS)
	$(CC) -o bin/clox $(OBJECTS)

test: clox tests/scanner_tests.c
	$(CC) -o tests/scanner_tests tests/scanner_tests.c
	sh ./tests/runtests.sh

















