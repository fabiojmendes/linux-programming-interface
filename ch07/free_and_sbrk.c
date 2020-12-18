/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 7-1 */

/* free_and_sbrk.c

   Test if free(3) actually lowers the program break.

   Usage: free_and_sbrk num-allocs block-size [step [min [max]]]

   Try: free_and_sbrk 1000 10240 2 1 1000
        free_and_sbrk 1000 10240 1 1 999
        free_and_sbrk 1000 10240 1 500 1000

        (Only the last of these should see the program break lowered.)
*/
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ALLOCS 1000000

extern char etext, edata, end;

void usage_err(char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  exit(1);
}

void fatal_err(char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  perror(buffer);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 3 || strcmp(argv[1], "--help") == 0) {
    usage_err("%s num-allocs block-size [step [min [max]]]\n", argv[0]);
  }

  int numAllocs = atoi(argv[1]);
  if (numAllocs > MAX_ALLOCS) {
    usage_err("num-allocs > %d\n", MAX_ALLOCS);
  }

  int blockSize = atoi(argv[2]);

  int free_stp = (argc > 3) ? atoi(argv[3]) : 1;
  int free_min = (argc > 4) ? atoi(argv[4]) : 1;
  int free_max = (argc > 5) ? atoi(argv[5]) : numAllocs;

  if (free_max > numAllocs) {
    usage_err("free-max > num-allocs\n");
  }

  void *brk = sbrk(0);
  printf("Initial program break:          %10p\n", brk);

  char *ptr[MAX_ALLOCS];
  printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
  for (int i = 0; i < numAllocs; i++) {
    ptr[i] = malloc(blockSize);
    if (ptr[i] == NULL) {
      fatal_err("malloc");
    }
    void *cbrk = sbrk(0);
    if (cbrk != brk) {
      printf("New program break[%06d]:          %10p (+%d kbytes)\n", //
             i, cbrk, (cbrk - brk) / 1024);
      brk = cbrk;
    }
  }

  printf("Program break is now:           %10p\n", brk);

  printf("Freeing blocks from %d to %d in steps of %d\n", //
         free_min, free_max, free_stp);
  for (int i = free_max - 1; i >= 0; i -= free_stp) {
    free(ptr[i]);
    void *cbrk = sbrk(0);
    if (cbrk != brk) {
      printf("During free[%06d], program break is: %10p (-%d bytes)\n",

             i, cbrk, (brk - cbrk));
      brk = cbrk;
    }
  }

  printf("After free(), program break is: %10p\n", brk);

  exit(EXIT_SUCCESS);
}
