CC=gcc -g
objects = main.o routes.o dev_tracker.o perms_changer.o backup.o transfer.o queue.o
headers = main.c routes.h dev_tracker.h perms_changer.h backup.h transfer.h queue.h

myprog : $(objects)
				$(CC) -o bin/change_tracker $(objects) -lm -lrt

main.o : main.c $(headers)
				$(CC) -c main.c -lrt
								
routes.o : routes.c
				$(CC) -c routes.c -lm

dev_tracker.o : dev_tracker.c
				$(CC) -c dev_tracker.c -lm
				
perms_changer.o : perms_changer.c
				$(CC) -c perms_changer.c -lm
				
backup.o : backup.c
				$(CC) -c backup.c -lm

transfer.o : transfer.c
				$(CC) -c transfer.c -lm
				
queue.o : queue.c
				$(CC) -c queue.c -lm

clean:
				rm bin/change_tracker $(objects)