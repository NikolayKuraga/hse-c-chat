#compiler
CC=i686-w64-mingw32-gcc

#flags for compiler
CFLAGS= -g -Wall -fmax-errors=1 -Wno-unknown-pragmas

server-client-launcher: a.exe

shared.o: shared.c shared.h
	$(CC) $(CFLAGS) -c shared.c

client.o: client.c client.h shared.h
	$(CC) $(CFLAGS) -c client.c

server.o: server.c server.h shared.h
	$(CC) $(CFLAGS) -c server.c

main.o: server.h client.h
	$(CC) $(CFLAGS) -c main.c

a.exe: main.o server.o client.o shared.o
	$(CC) $(CFLAGS) main.o server.o client.o shared.o -static -lpthread -lws2_32 -o a.exe

clean:
	rm -rf *.o a.exe
