/*
  Write a C program to offer the following functionality:
  a. List all the processes running on a system if no params are
  passed via the command line
  b. Search for a process by name eg. ./myprog search
  calculator
  c. Kill a process for a given IPD rg. ./myprog kill 1292
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

int main(int argc, char* argv[]) {
  // List all processes running on system
  if (argc == 1) {
    char *args[]={"ps", "-aux", (char *)0};
    execv("/bin/ps", args);
    return 0;
  } 
  
  if (argc == 3) {
    // List process matching seatch term
    if (strcmp(argv[1], "search") == 0) {
      char *args[]={"pgrep", argv[2], NULL};
      execv("/usr/bin/pgrep", args);
    } else if(strcmp(argv[1], "kill") == 0) {
      char* args[] = {"kill", argv[2], NULL};
      execv("/bin/kill", args);
    } else {
      printf("No matching command");
    }
  }
  
  return 0;
}