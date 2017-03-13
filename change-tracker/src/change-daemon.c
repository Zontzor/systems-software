#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void exec1();
void exec2();
void exec3();

int pid;
int pipefd1[2];
int pipefd2[2];

char dev_dir[100] = "/home/alex/Coding/systems-software/change-tracker/website/dev/.";

void main() {
  
  int fd; /*file descriptor to the file we will redirect ls's output*/
  
  FILE *file = fopen("/home/alex/Coding/systems-software/change-tracker/admin/changes.txt", "w");
  
  fd = fileno(file);
  
  dup2(fd,STDOUT_FILENO); /*copy the file descriptor fd into standard output*/
  dup2(fd,STDERR_FILENO); /* same, for the standard error */
  close(fd);

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
    exec1();
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
    exec2();
  }

  // close unused fds
  close(pipefd1[0]);
  close(pipefd1[1]);

  // fork (sort)
  if ((pid = fork()) == -1) {
    perror("Error sort fork");
    exit(1);
  } else if (pid == 0) {
    exec3();
  }
}


void exec1() {
  // set the output to pipe 1
  dup2(pipefd1[1], 1);
  
  // close fds
  close(pipefd1[0]);
  close(pipefd1[1]);
  
  // exec
  execlp("find", dev_dir, "-mtime", "-1", "-type", "f", "-ls",  NULL);
  
  // exec didn't work, exit
  perror("Error with ls -al");
  _exit(1);
}

void exec2() {
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
  execlp("awk", "awk",  "{print $5\",\"$10\",\"$11}", NULL);
  
  // error check
  perror("bad exec grep root");
  _exit(1);
}

void exec3() {
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
}

