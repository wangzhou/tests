#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

void *thread_fun(void *arg)
{
	unsigned long num = 0;
	int fd, flags;	

	fd = open("/dev/fork0", O_RDWR | O_CLOEXEC);
	if (fd == -1) {
		printf("fail to open /dev/fork0\n");
		return NULL;
	}

	while (1) {
		num++;
	}
}

int main()
{
	pthread_t work_thread;
	pid_t pid;
	int fd;

	if (pthread_create(&work_thread, NULL, thread_fun, NULL)) {
		printf("fail to create thread\n");
		abort();
	}
	
	sleep(1);

	if ((pid = fork()) < 0) {
		printf("fail to fork\n");
	} else if (pid == 0) {
		printf("in child process %d\n", getpid());
		if (execlp("ls", "ls", "-al", NULL) == -1) {
			printf("fail to execlp\n");
		}
	} else {
		printf("in father process %d\n", getpid());
	}

	if (pthread_join(work_thread, NULL)) {
		printf("fail to joining thread\n");
		abort();
	}
}
