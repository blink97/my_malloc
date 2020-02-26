CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -I./src/headers -fPIC -D_GNU_SOURCE -g
VPATH = ./src

OBJ = malloc.o free.o calloc.o realloc.o
TESTFILE= tests/expected.txt tests/output.txt tests/diff.txt

libmalloc.so: ${OBJ}
	${CC} -shared -o libmalloc.so ${OBJ}

test: libmalloc.so
	./tests/testsuite.sh

clean:
	${RM} ${OBJ}
	${RM} libmalloc.so
	${RM} ${TESTFILE}
