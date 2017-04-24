CC=gcc -g
objects = main.o connection_handler.o
headers = main.c connection_handler.h

myprog : $(objects)
				$(CC) -o bin/file_transfer_server $(objects) -lpthread

main.o : main.c $(headers)
				$(CC) -c main.c -lrt

connection_handler.o : connection_handler.c
				$(CC) -c connection_handler.c -lm

clean:
				rm bin/file_transfer_server $(objects)
