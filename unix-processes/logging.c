/*
  Create a program that manages the process of automatically
  creating a log file in a directory. This should be a separate
  process. If this process terminates unexpectedly the main
  program should be informed via an appropriate signal.
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>

void signal_handler(int sigNum) {
  if (sigNum == SIGINT) {
    printf("SIGINT interrupt recieved");
  }
}

int main(void) {
  printf("Start\n");
  
  pid_t pid = fork();
  
  /* Check for child process */
  if (pid == 0) {
    FILE *file = fopen("bin/data.log", "w");
    fclose(file);
  } else { /* pid != 0 and is the parent process */
    while(1) {
      printf("\nParent here...");
      sleep(1);
    }
  }
  
  return 0;
}