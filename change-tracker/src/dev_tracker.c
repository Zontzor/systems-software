#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "dev_tracker.h"

void exec1();
void exec2();
void exec3();

int pid;
int pipefd1[2];
int pipefd2[2];

char dev_dir[100] = "/home/alex/Coding/systems-software/change-tracker/website/dev/";
char changes_dir[100] = "/home/alex/Coding/systems-software/change-tracker/admin/dev_changes.txt";

void get_changes();

void dev_change_tracker() {
  int fd;

  FILE *file = fopen(changes_dir, "w");

  fd = fileno(file);

  dup2(fd,STDOUT_FILENO);
  dup2(fd,STDERR_FILENO);
  close(fd);

  while(1) {
    get_changes();
    sleep(1);
  }
}

void get_changes() {
  char foo[4096];

  // create pipe between find and awk
  if (pipe(pipefd1) == -1) {
    perror("Error Init Pipe");
    exit(1);
  }

  // fork (ls aux)
  if ((pid = fork()) == -1) {
    perror("Error find fork");
    exit(1);
  } else if (pid == 0) {
    dup2(pipefd1[1], 1);

    // close fds
    close(pipefd1[0]);
    close(pipefd1[1]);

    // exec
    execlp("find", "find", dev_dir, "-cmin", "-0.017", "-type", "f", "-ls",  NULL);

    // exec didn't work, exit
    perror("Error with ls -al");
    _exit(1);
  }

  // create pipe between awk and sort
  if (pipe(pipefd2) == -1) {
    perror("Error Init Pipe");
    exit(1);
  }

  // fork (awk)
  if ((pid = fork()) == -1) {
    perror("Error awk fork");
    exit(1);
  } else if (pid == 0) {
    // get the input from pipe 1
    dup2(pipefd1[0], 0);

    // set the output to pipe 2
    dup2(pipefd2[1], 1);

    // close fds
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);

    // exec
    execlp("awk", "awk", "{print $5,$8,$9,$10,$11}", NULL);

    // error check
    perror("bad exec grep root");
    _exit(1);
  }

  // close unused fds
  close(pipefd1[0]);
  close(pipefd1[1]);

  // fork (sort)
  if ((pid = fork()) == -1) {
    perror("Error sort fork");
    exit(1);
  } else if (pid == 0) {
    // get the input from pipe 2
    dup2(pipefd2[0], 0);

    // close fds
    close(pipefd2[0]);
    close(pipefd2[1]);
    // exec
    execlp("sort", "sort", "-u", NULL);

    // error Check
    perror("Error with sort");
    _exit(1);
  } else {
    close(pipefd2[1]);
    int nbytes = read(pipefd2[0], foo, sizeof(foo));
    printf("%.*s", nbytes, foo);
    close(pipefd2[0]);
  }
}
