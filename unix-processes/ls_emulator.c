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