CC=gcc -g
objects = main.o
headers = main.c

myprog : $(objects)
				$(CC) -o bin/file_transfer_client $(objects) -lm -lrt

main.o : main.c $(headers)
				$(CC) -c main.c -lrt

clean:
				rm bin/file_transfer_client $(objects)