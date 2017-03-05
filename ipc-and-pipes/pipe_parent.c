#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int fd[2], nbytes;
  pid_t pid;
  char string[] = "Hey bud!!\n";
  char readbuffer[100];
  
  // Create a pipe
  pipe(fd);
  
  // Create a child process
  pid = fork();
  
  
  // Setup child file descriptors
  if (pid == 0) {
    // Take no input, close fd[0]
    close(fd[0]);
    
    // Send output to parent via write
    write(fd[1], string, (strlen(string) + 1));
    exit(0);
  } else { // This is the parent
    // Send no output, close fd[1]
    close(fd[1]);
    
    // Get input from the pipe via read
    nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
    printf("Message from Child: %s", readbuffer);
  }
}
