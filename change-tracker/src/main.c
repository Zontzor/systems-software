#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "dev_tracker.h"

void daemonize();

int main() {
  daemonize();
    
  dev_change_tracker();
    
  int i = 0;
  while(i < 30) {
    // TODO: Queue logic
    i++;
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
