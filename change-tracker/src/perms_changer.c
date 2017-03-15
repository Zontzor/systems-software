#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "routes.h"
#include "perms_changer.h"

void lock_dir() {
  char mode[4] = "0000";
  int i = strtol(mode, 0, 8);
  chmod(dev_dir, i);
}

void unlock_dir() {
  char mode[4] = "0777";
  int i = strtol(mode, 0, 8);
  chmod(dev_dir, i);
}
