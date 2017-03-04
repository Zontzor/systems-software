/*
	Write a C program to emulate the ls -l UNIX command that
	prints all files in a current directory. Hint: Use the exec
	command.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  char *argv[]={"ls", "-l", (char *)0};
  execv("/bin/ls", argv);
  return 0;
}