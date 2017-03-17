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
#include <string.h>
#include <time.h>
#include <mqueue.h>
#include "daemonize.h"
#include "dev_tracker.h"
#include "perms_changer.h"
#include "backup.h"
#include "transfer.h"

int main() {
  daemonize();
  
  time_t now;
  struct tm newyear;
  double seconds;
  time(&now);
  newyear = *localtime(&now);
  newyear.tm_hour = 0; 
  newyear.tm_min = 14; 
  newyear.tm_sec = 0;
  
  //dev_tracker();
  
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
    } else {
      dev_tracker();
    }
    
    i++;
    sleep(1);
  }

  return 0;
}


