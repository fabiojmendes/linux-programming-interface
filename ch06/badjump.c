#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>

jmp_buf env;

int f2(int p1, int p2) {
  if (p1 == 0 || p2 ==0)
    longjmp(env, 1);
  return 1;
}

int f1(int p1, int p2) {
  volatile int result = (p1 + p2) * 100;
  if(setjmp(env)) {
    return -1;
  }
  f2(p1, p2);
  return result;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    return EXIT_FAILURE;
  }
  int p1 = atoi(argv[1]);
  int p2 = atoi(argv[2]);
  int res = f1(p1, p2);
  printf("Result: %d\n", res);
  return EXIT_SUCCESS;
}
