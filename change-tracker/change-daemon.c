#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
     
int  main (int argc, char **argv) {
  // Create a child process
  int pid = fork();

  // If PID > 0, then it is the parent
  if (pid > 0) {
    printf("Parent process\n");
    // sleep(5);
    exit(EXIT_SUCCESS);
  } else if (pid == 0) {
    // Step 1: Create the orphan process
    printf("Child process\n");

    /*  
      Step 2: Elevate the orphan process to session leader, to lose
      controlling TTY (terminal).   
      This command runs the process in a new session
    */
    if (setsid() < 0) {
      exit(EXIT_FAILURE);
    };
    
    /*
      Step 3: Call umask() to set the file mode creation mask to 0.
      This will allow the daemon to rear and write file with the permissions/
      access required
    */
    umask(0);
    
    /*
      Step 4: Change the current working dit to root.
      This will eliminate any issue of running on a mounted drive,
      that potentially could be removed, etc...
    */
    if (chdir("/") < 0) {
      exit(EXIT_FAILURE);
    };
    
    /*
      Step 5: Close all open file descriptors
    */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
      close(x);
    }

    while(1) {
      sleep(1);
      printf("Child 1: My parent is: %i\n", getppid());
    }
  }

  return 0;
}