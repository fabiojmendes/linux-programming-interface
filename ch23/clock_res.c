#include <stdio.h>
#include <time.h>

int main() {
  printf("Resolutions\n");
  struct timespec t;

  clock_getres(CLOCK_REALTIME, &t);
  printf("CLOCK_REALTIME %ld, %ld\n", t.tv_sec, t.tv_nsec);

  clock_getres(CLOCK_MONOTONIC, &t);
  printf("CLOCK_MONOTONIC %ld, %ld\n", t.tv_sec, t.tv_nsec);

  clock_getres(CLOCK_PROCESS_CPUTIME_ID, &t);
  printf("CLOCK_PROCESS_CPUTIME_ID %ld, %ld\n", t.tv_sec, t.tv_nsec);

  return 0;
}
