COMPILER = gcc
CFLAGS = -Wall -pedantic
COBJS = thread_handling.o rdrsaddmul_utils.o

EXES = rdrsaddmul

all: ${EXES}

rdrsaddmul:	rdrsaddmul.c ${COBJS}
	${COMPILER} ${CFLAGS} rdrsaddmul.c ${COBJS} -o rdrsaddmul -pthread

thread_handling.o: thread_handling.c thread_handling.h
	${COMPILER} ${CFLAGS} -c thread_handling.c

rdrsaddmul_utils.o: rdrsaddmul_utils.c rdrsaddmul_utils.h
	${COMPILER} ${CFLAGS} -c rdrsaddmul_utils.c

run: rdrsaddmul
	./rdrsaddmul

clean:
	rm -f *~ *.o ${EXES}