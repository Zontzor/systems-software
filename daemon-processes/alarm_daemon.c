/*
  Create a	C	program that can compare two	times	and	countdown	to a specific 
  time.	
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
  // Create a child process
  int pid = fork();

  // If PID > 0, then it is the parent
  if (pid > 0) {
    printf("Parent process\n");
    // sleep(5);
    exit(EXIT_SUCCESS);
  } else if (pid == 0) {
    // Step 1: Create the orphan process
    printf("Child process\n");

    /*  
      Step 2: Elevate the orphan process to session leader, to lose
      controlling TTY (terminal).   
      This command runs the process in a new session
    */
    if (setsid() < 0) {
      exit(EXIT_FAILURE);
    };
    
    /*
      Step 3: Call umask() to set the file mode creation mask to 0.
      This will allow the daemon to rear and write file with the permissions/
      access required
    */
    umask(0);
    
    /*
      Step 4: Change the current working dir to root.
      This will eliminate any issue of running on a mounted drive,
      that potentially could be removed, etc...
    */
    if (chdir("/") < 0) {
      exit(EXIT_FAILURE);
    };
    
    /*
      Step 5: Close all open file descriptors
    */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
      close(x);
    }

    while(1) {
      time_t now = time(NULL);
      struct tm newyear;
      newyear = *localtime(&now);
      newyear.tm_hour = 15;
      newyear.tm_min = 28;
      newyear.tm_sec = 0;
      
    	char* c_time_string = ctime(&now);	
  		int time_diff = difftime(mktime(&newyear), now);
      
      if (time_diff == 0) {
        FILE *file = fopen("/home/alex/Coding/systems-software/daemon-processes/bin/alarm.txt", "w");
        fprintf(file, "%s", "Wake up!");
        fprintf(file, "\nMy process id: %i", getpid());			
        fprintf(file, "\nMy parent process id: %i", getppid());	
        fprintf(file, "\nMy group id: %i", getpgrp());	
        fclose(file);
      }	
  		sleep(1);
    }
  }

  return 0;
}