/*
  Authour: Alex Kiernan
  Date: 14/03/17
  
  Desc: Module responsible for backing up live dir. Names backup dir as 
        timestamp.
**/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "routes.h"
#include  <sys/types.h>

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
    exit(1);
  } else if (pid == 0) {
    char *command = "/bin/cp";
    char *arguments[] = { "cp", "-a", local_live_dir, local_backup_dir, NULL };
    execvp(command, arguments);

    // error check
    perror("Error with cp");
    _exit(1);
  }
}