#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "routes.h"
#include <sys/types.h>

void get_transfers();

void transfer() {
  get_transfers();
  
  char local_transfers_dir[100];
  strcpy(local_transfers_dir, transfers_dir);
  
  char local_live_html_dir[100];
  strcpy(local_live_html_dir, live_html_dir);
  
  FILE * fp;
  
  int i = 0;
  int num_lines = 0;
  
  fp = fopen(local_transfers_dir, "r");
  if (fp == NULL) {
    printf("Could not open file");
    exit(EXIT_FAILURE);
  }
  
  char records[10][100];
  
  while(fscanf(fp, "%s", records[i]) != EOF) {
    i++;
  }
  
  fclose(fp);
  
  num_lines = i;
  
  for (int i = 0; i < num_lines; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      char *command = "/bin/cp";
      char *arguments[] = {"cp", "-f", records[i], local_live_html_dir, NULL};
      execvp(command, arguments);
    } else {
      
    }
  }
}

void get_transfers() {
  int pid;
  int pipefd[2];

  char foo[4096];

  int fd;

  FILE *file = fopen(transfers_dir, "w");

  fd = fileno(file);

  dup2(fd,STDOUT_FILENO);
  dup2(fd,STDERR_FILENO);
  close(fd);
  
  pipe(pipefd);

  // fork (find)
  if ((pid = fork()) == -1) {
    perror("Error find fork");
    exit(1);
  } else if (pid == 0) {
    close(pipefd[1]);
    // exec
    execlp("find", "find", dev_html_dir, "-mtime", "-1", "-type", "f", NULL);

    // exec didn't work, exit
    perror("Error with ls -al");
    _exit(1);
  } else {
    int nbytes = read(pipefd[0], foo, sizeof(foo));
    printf("%.*s", nbytes, foo);
    close(pipefd[0]);
  }
}