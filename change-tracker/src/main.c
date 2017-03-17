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
  
  // Create target time struct
  time_t now;
  struct tm newyear;
  double seconds;
  time(&now);
  
  newyear = *localtime(&now);
  newyear.tm_hour = 0; 
  newyear.tm_min = 21; 
  newyear.tm_sec = 0;
  
  // Create queue
  mqd_t mq;
  struct mq_attr queue_attributes;
  char buffer[1024 + 1];
  int terminate = 0;

  queue_attributes.mq_flags = 0;
  queue_attributes.mq_maxmsg = 10;
  queue_attributes.mq_msgsize = 1024;
  queue_attributes.mq_curmsgs = 0;

  mq = mq_open("/ct_queue", O_CREAT | O_RDONLY, 0644, &queue_attributes); 
  
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
  
  mq_close(mq);
  mq_unlink("/ct_queue");

  return 0;
}


