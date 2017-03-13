#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  char dev_dir[100] = "/home/alex/Coding/systems-software/change-tracker/website/dev/.";
  
  int fd; /*file descriptor to the file we will redirect ls's output*/
  
  FILE *file = fopen("/home/alex/Coding/systems-software/change-tracker/admin/changes.txt", "w");
  
  fd = fileno(file);
  
  dup2(fd,STDOUT_FILENO); /*copy the file descriptor fd into standard output*/
  dup2(fd,STDERR_FILENO); /* same, for the standard error */
  close(fd);
  
  char *command = "/usr/bin/find";
  char *arguments[] = { "find", dev_dir, "-mtime", "-1", "-type", "f", "-ls",  NULL };
  execvp(command, arguments);
  
  return 0;
}
