/*
  Using the pipe and fork commands, create a child process that can send a 
  message to its parent
*/

#include <stdio.h>
#include <unistd.h>
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
    close(fd[1]);

    // Get input from the pipe via read
    nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
    
    printf("Message from Parent: %s", readbuffer);
  } else { // This is the parent
    // Send no output, close fd[1]
    close(fd[0]);

    // Send output to child via write
    write(fd[1], string, (strlen(string) + 1));
    exit(0);
  }
}
