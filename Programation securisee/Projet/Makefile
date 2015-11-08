CC=gcc
CFLAGS=-W -Wall -ansi 
EXEC=DynStat

all: $(EXEC)

DynStat: DynStat.o
	$(CC) -o DynStat DynStat.o

DynStat.o: DynStat.c
	$(CC) -o DynStat.o -c DynStat.c

clean:
	rm -rf *.o