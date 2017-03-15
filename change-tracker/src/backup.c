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
  
  char local_dev_dir[100];
  strcpy(local_dev_dir, dev_dir);
  
  char local_backup_dir[100];
  strcpy(local_backup_dir, backup_dir);

  t = time(NULL);
  tm = localtime(&t);
  
  // Get a string timestamp
  strftime(str_timestamp, sizeof(str_timestamp), "%Y%m%d%H%M%S", tm);
  
  // Create a new backup dir with name of timestamp
  strcat(local_backup_dir, str_timestamp);
  
  pid_t pid;
  
  // fork (ls aux)
  if ((pid = fork()) == -1) {
    perror("Error cp fork");
    exit(1);
  } else if (pid == 0) {
    char *command = "/bin/cp";
    char *arguments[] = { "cp", "-a", local_dev_dir, local_backup_dir, NULL };
    execvp(command, arguments);

    // exec didn't work, exit
    perror("Error with cp");
    _exit(1);
  }
}