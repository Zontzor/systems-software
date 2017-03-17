/*
  Authour: Alex Kiernan
  Date: 14/03/17
  
  Desc: Manager for website change tracker. 
    
    1. Create daemon
    2. Every second, check if correct time to execute backup/transfer, else
       check for changes to dev
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "dev_tracker.h"
#include "perms_changer.h"
#include "backup.h"
#include "transfer.h"
#include "queue.h"

int main() {
  int pid = fork();

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  } else if (pid == 0) {
    printf("Starting child tracker process\n");

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
  
    queue();
  
    // Create target time struct
    time_t now;
    struct tm newyear;
    double seconds;
    time(&now);
    
    newyear = *localtime(&now);
    newyear.tm_hour = 11; 
    newyear.tm_min = 59; 
    newyear.tm_sec = 0;
    
    int i = 0;
    while(i < 60) {
      time(&now);
      seconds = difftime(now, mktime(&newyear));
      if (seconds == 0) {
        lock_dir();
        backup();
        transfer();
        unlock_dir();
      } else {
        dev_tracker();
      } 
      
      i++;
      sleep(1);
    }
  }
  
  return 0;
}
