LIB_NAME=qlist

CFLAGS=-g -O3 -Wall -Isrc

SRC=$(wildcard src/*.c src/**/*.c)
OBJ=$(patsubst %.c,%.o,${SRC})

LIB_OBJ=${OBJ}

TEST_SRC=$(wildcard test/*_test.c)
TESTS=$(patsubst %.c,%,${TEST_SRC})

TEST_HELPER_SRC=$(wildcard test/helpers/*.c)
TEST_HELPER_OBJ=$(patsubst %.c,%.o,${TEST_HELPER_SRC})


all: clean build/${LIB_NAME}.a $(TESTS)
debug: CFLAGS+=-DDEBUG
debug: all

build/${LIB_NAME}.a: build ${LIB_OBJ}
	ar rvs $@ ${LIB_OBJ}
	ranlib $@

build:
	@mkdir -p build

clean:
	rm -rf build
	rm -rf ${OBJ}
	rm -rf ${TEST_HELPER_OBJ}
	rm -rf ${TESTS}
	rm -rf test/*.dSYM
	rm -rf tmp/*

${TESTS}: %: %.c build/${LIB_NAME}.a ${TEST_HELPER_OBJ}
	$(CC) $(CFLAGS) -Itest -o $@ $< build/${LIB_NAME}.a ${TEST_HELPER_OBJ}

tests: all ${TESTS}
	./test/function_test

memcheck: all ${TESTS}
	valgrind --leak-check=full --show-reachable=no ./test/function_test