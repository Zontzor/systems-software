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
#include <syslog.h>
#include "daemonize.h"
#include "dev_tracker.h"
#include "perms_changer.h"
#include "backup.h"
#include "transfer.h"

#define QUEUE_NAME "/ct_queue"

int queue();

int main() {
  daemonize();
  //queue();
  
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

  return 0;
}

int queue() {
  daemonize();
  
  mqd_t mq;
  struct mq_attr queue_attributes;
  char buffer[1024 + 1];
  int terminate = 0;

  queue_attributes.mq_flags = 1;
  queue_attributes.mq_maxmsg = 10;
  queue_attributes.mq_msgsize = 1024;
  queue_attributes.mq_curmsgs = 0;

  mq = mq_open("/ct_queue", O_CREAT | O_RDONLY, 0644, &queue_attributes); 

  do {
    ssize_t bytes_read;
    
    bytes_read = mq_receive(mq, buffer, 1024, NULL);
    
    buffer[bytes_read] = '\0';
    if (!strncmp(buffer, "exit", strlen("exit")))
    {terminate = 1;}
    else 
    {printf("Recieved: %s\n", buffer);}
    
    sleep(1);
  } while(!terminate);

  mq_close(mq);
  mq_unlink(QUEUE_NAME);
  
  return 0;
}
