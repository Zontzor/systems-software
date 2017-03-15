#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

char live_dir[100] = "/home/alex/Coding/systems-software/change-tracker/website/live/var/www/html/";
char changes_dir[100] = "/home/alex/Coding/systems-software/change-tracker/admin/changes.txt";


int main(int argc, char* argv[]) {
  FILE * fp;
  
  int i = 0;
  int num_lines = 0;
  
  fp = fopen(changes_dir, "r");
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
      char *arguments[] = {"cp", "-f", records[i], live_dir, NULL};
      execvp(command, arguments);
    } else {
      
    }
  }
  
  return 0;
}
