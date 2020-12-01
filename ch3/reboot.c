#include <linux/reboot.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <unistd.h>

int sum(int p1, int p2) { return p1 + p2; }

int main() {
  printf("Hello there!\n");
  printf("  %#x\n", LINUX_REBOOT_MAGIC1);
  printf("  %#x\n", LINUX_REBOOT_MAGIC2);
  printf("  %#x\n", LINUX_REBOOT_MAGIC2A);
  printf("  %#x\n", LINUX_REBOOT_MAGIC2B);
  printf("  %#x\n", LINUX_REBOOT_MAGIC2C);
  return sum(1, 2) - 3;
}
