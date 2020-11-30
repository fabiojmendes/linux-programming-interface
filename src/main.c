#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

int sum(int p1, int p2)
{
    return p1 + p2;
}

int main()
{
    printf("Hello there!\n");
    return sum(1, 2) - 3;
}
