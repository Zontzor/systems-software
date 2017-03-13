#include <stdio.h>
#include <stdlib.h>

char dev_dir[100] = "/home/alex/Coding/systems-software/change-tracker/website/dev";
char live_dir[100] = "/home/alex/Coding/systems-software/change-tracker/website/live";
char changes_dir[100] = "/home/alex/Coding/systems-software/change-tracker/admin/changes.txt";

int main(int argc, char* argv[]) {
  FILE *file = fopen(changes_dir, "r");
  
  
  return 0;
}
