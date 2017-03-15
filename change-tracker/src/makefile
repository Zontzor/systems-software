CC=gcc -g
objects = main.o dev_tracker.o
headers = main.c dev_tracker.h

myprog : $(objects)
				$(CC) -o change_tracker $(objects) -lm

main.o : main.c $(headers)
				$(CC) -c main.c

dev_tracker.o : dev_tracker.c
				$(CC) -c dev_tracker.c -lm

clean:
				rm change_tracker $(objects)