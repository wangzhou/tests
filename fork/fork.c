#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

int count = 1;

int main()
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		printf("fail to fork\n");
	} else if (pid == 0) {
		printf("in child process %d\n", count);
		printf("in child process %d\n", count);
	} else {
		printf("in father process %d\n", count);
	}
}
