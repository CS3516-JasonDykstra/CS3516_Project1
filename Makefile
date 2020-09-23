all: client.o server.o client server

client.o: client.c
	gcc -c client.c client.h

server.o: server.c
	gcc -c server.c server.h

client: client.o
	gcc client.o -o client

server: server.o
	gcc server.o -o server

clean:
	rm *.o *.h.gch client server