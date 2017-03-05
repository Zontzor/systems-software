/*
  Create a	C	program that can compare two	times	and	countdown	to a specific 
  time.	
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define PATH "/home/alex/Coding/systems-software/daemon-processes/bin/alarm.txt"

int main() {
  // Create a child process
  int pid = fork();

  // If PID > 0, then it is the parent
  if (pid > 0) {
    printf("Parent process\n");
    // sleep(5);
    exit(EXIT_SUCCESS);
  } else if (pid == 0) {
    printf("Child process\n");
    
    if (setsid() < 0) {
      exit(EXIT_FAILURE);
    };
    
    umask(0);
    
    if (chdir("/") < 0) {
      exit(EXIT_FAILURE);
    };
  
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
      close(x);
    }

    while(1) {
      time_t now = time(NULL);
      struct tm newyear;
      
      newyear = *localtime(&now);
      newyear.tm_hour = 9;
      newyear.tm_min = 0;
      newyear.tm_sec = 0;
  		int time_diff_am = difftime(mktime(&newyear), now);
      
      newyear = *localtime(&now);
      newyear.tm_hour = 17;
      newyear.tm_min = 0;
      newyear.tm_sec = 0;
      int time_diff_pm = difftime(mktime(&newyear), now);
      
      if (time_diff_am == 0) {
        char mode[4]="0000";
        int i;
        i = strtol(mode, 0, 8);
        chmod(PATH, i);
      }	else if (time_diff_pm == 0) {
        char mode[4]="0777";
        int i;
        i = strtol(mode, 0, 8);
        chmod(PATH, i);
      }
      
  		sleep(1);
    }
  }

  return 0;
}