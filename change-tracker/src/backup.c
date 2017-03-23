/*
  Authour: Alex Kiernan
  Date: 14/03/17
  
  Desc: Module responsible for backing up live dir. Names backup dir as 
        timestamp.
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <mqueue.h>
#include <time.h>
#include "routes.h"
#include <sys/types.h>
#include <sys/wait.h>

#define QUEUE_NAME "/ct_queue"

void backup() {
  struct tm *tm;
  time_t t;
  char str_timestamp[100];
  
  char local_live_dir[100];
  strcpy(local_live_dir, live_dir);
  
  char local_backup_dir[100];
  strcpy(local_backup_dir, backup_dir);

  t = time(NULL);
  tm = localtime(&t);
  
  // Get a string timestamp
  strftime(str_timestamp, sizeof(str_timestamp), "%Y%m%d%H%M%S", tm);
  
  // Create a new backup dir with name of timestamp
  strcat(local_backup_dir, str_timestamp);
  
  pid_t pid;
  
  // fork (cp)
  if ((pid = fork()) == -1) {
    perror("Error cp fork");
    
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Backup error");
    closelog();
    
    exit(1);
  } else if (pid == 0) {
    char *command = "/bin/cp";
    char *arguments[] = { "cp", "-a", local_live_dir, local_backup_dir, NULL };
    execvp(command, arguments);

    // error check
    perror("Error with cp");
    _exit(1);
  } else {
    int status;
    pid = wait(&status);
    if (WIFEXITED(status)) {
      // Connect to queue as client
      mqd_t mq;
      char buffer[1024];
      mq = mq_open(QUEUE_NAME, O_WRONLY);
      mq_send(mq, "backup_success", 1024, 0);
      mq_close(mq);
      
      
      openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "Backup success");
      closelog();
    } else {
      mqd_t mq;
      char buffer[1024];
      mq = mq_open(QUEUE_NAME, O_WRONLY);
      mq_send(mq, "backup_failure", 1024, 0);
      mq_close(mq);
      
      openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "Backup failure");
      closelog();
    }
  }
}