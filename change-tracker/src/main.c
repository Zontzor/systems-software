/*
  Authour: Alex Kiernan
  Date: 14/03/17
  
  Desc: Manager for website change tracker. 
    
    1. Create daemon
    2. Start dev dir change tracker
    3. Every second, check if correct time to execute backup/transfer
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "dev_tracker.h"
#include "perms_changer.h"
#include "backup.h"
#include "transfer.h"

void daemonize();

int main() {
  daemonize();
    
  //dev_tracker();
  
  time_t now;
  struct tm newyear;
  double seconds;
  time(&now);
  newyear = *localtime(&now);
  newyear.tm_hour = 22; 
  newyear.tm_min = 26; 
  newyear.tm_sec = 0;
  
  int i = 0;
  while(i < 60) {
    // TODO: Queue logic
    
    time(&now);
    seconds = difftime(now, mktime(&newyear));
    if (seconds == 0) {
      lock_dir();
      backup();
      transfer();
      unlock_dir();
    }
    
    i++;
    sleep(1);
  }

  return 0;
}

void daemonize() {
  int pid = fork();

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  } else if (pid == 0) {
    printf("Starting child process\n");

    if (setsid() < 0) {
      exit(EXIT_FAILURE);
    };

    umask(0);

    if (chdir("/") < 0) {
      exit(EXIT_FAILURE);
    };
  }
}
