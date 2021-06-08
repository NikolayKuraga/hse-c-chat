# compiler
CC=gcc

# flags for compiler
CFLAGS= -Wall -Werror -fmax-errors=1

project: a.out

shared.o: shared.c shared.h
	$(CC) $(CFLAGS) -c shared.c 

client.o: client.c client.h shared.h
	$(CC) $(CFLAGS) -c client.c

server.o: server.c server.h shared.h
	$(CC) $(CFLAGS) -c server.c

main.o: server.h client.h
	$(CC) $(CFLAGS) -c main.c

a.out: main.o server.o client.o shared.o
	$(CC) $(CFLAGS) -lpthread main.o server.o client.o shared.o

clean:
	rm -rf *.o a.out a.exe
