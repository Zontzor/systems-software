CC=gcc -g
objects = main.o routes.o dev_tracker.o perms_changer.o
headers = main.c routes.h dev_tracker.h perms_changer.h

myprog : $(objects)
				$(CC) -o bin/change_tracker $(objects) -lm

main.o : main.c $(headers)
				$(CC) -c main.c
				
routes.o : routes.c
				$(CC) -c routes.c -lm

dev_tracker.o : dev_tracker.c
				$(CC) -c dev_tracker.c -lm
				
perms_changer.o : perms_changer.c
				$(CC) -c perms_changer.c -lm

clean:
				rm bin/change_tracker $(objects)