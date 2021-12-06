#include <stdio.h>
#include <sys/select.h>

int main() {
  fd_set set;
  FD_ZERO(&set);
  FD_SET(0, &set);
  FD_SET(1, &set);

  printf("set: %d\n", set);
}