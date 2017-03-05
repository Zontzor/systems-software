/*
  Using	the	popen	command, recreate the basic pipes
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *fp1, *fp2;
  int status;
  char buffer1[1024];
  char buffer2[1024];
  
  fp1 = popen("Hi", "w");
  while (fgets(buffer1, 1024, fp1) != NULL) {
    printf("%s", buffer1);
  }
  
  fp2 = popen("Hey", "w");
  while (fgets(buffer2, 1024, fp2) != NULL) {
    printf("%s", buffer2);
  }
  
  status = pclose(fp1);
  status = pclose(fp2);
}
