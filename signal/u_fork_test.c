#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		printf("fail to fork\n");
	} else if (pid == 0) {
		while (1) {
			sleep(1);
			printf("in child process %d\n", getpid());
		}
	} else {
		printf("in father process %d\n", getpid());
		sleep(5);
		kill(pid, SIGBUS);
	}
}
