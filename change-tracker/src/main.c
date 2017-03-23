/*
  Authour: Alex Kiernan
  Date: 14/03/17
  
  Desc: Manager for website change tracker. 
    
    1. Create daemon
    2. Every second, check if correct time to execute backup/transfer, else
       check for changes to dev
    3. Register signals to accept user invocation
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "dev_tracker.h"
#include "perms_changer.h"
#include "backup.h"
#include "transfer.h"
#include "queue.h"

void signal_handler(int sig_no);

int main() {
  int pid = fork();

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  } else if (pid == 0) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Started change tracker process");
    closelog();

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
    struct tm midnight;
    double seconds;
    time(&now);
    
    midnight = *localtime(&now);
    midnight.tm_hour = 0; 
    midnight.tm_min = 0; 
    midnight.tm_sec = 0;
    
    // Register SIGINT to kernel
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
      openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "SIGINT catch error");
      closelog();
    }
    
    // Register SIGUSR1 to kernel
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
      openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "SIGUSR1 catch error");
      closelog();
    }
    
    while(1) {
      time(&now);
      seconds = difftime(now, mktime(&midnight));
      if (seconds == 0) {      
        lock_dir();
        backup();
        transfer();
        unlock_dir();
      } else {
        dev_tracker();
      } 
      sleep(1);
    }
  }
  
  return 0;
}

void signal_handler(int sig_no) {
  if (sig_no == SIGINT) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "SIGINT interrupt recieved");
    closelog();
  } else if (sig_no == SIGUSR1) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "SIGUSR1 interrupt recieved");
    closelog();
    
    lock_dir();
    backup();
    transfer();
    unlock_dir();
  }
}
