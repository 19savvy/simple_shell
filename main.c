#include "main.h"
#include <unistd.h>
#include <stdio.h>
int main(int ac, char **argv)
{
	pid_t ppid;
	ppid = getppid();
	printf("%u\n", ppid);
	return (0);
}
