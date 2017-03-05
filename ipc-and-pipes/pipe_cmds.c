/*
  Using the pipe and fork commands, create a program that uses the pipe command 
  for: ls -al|awk '{ print $3 }'|sort -u
*/

#include <stdlib.h>
#include <unistd.h>

void exec1();
void exec2();
void exec3();

int pid;
int pipefd1[2];
int pipefd2[2];

void main() {

  // create pipe between ls and awk
  if (pipe(pipefd1) == -1) {
    perror("Error Init Pipe");
    exit(1);
  }

  // fork (ls aux)
  if ((pid = fork()) == -1) {
    perror("Error ls fork");
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
  execlp("ls", "ls", "-al", NULL);
  
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
  execlp("awk", "awk", "{print $3}", NULL);
  
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
