#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	unsigned long num = 0;
	int fd, ret;	

	sigset_t set, old_set;

	if (sigemptyset(&set) == -1 || sigemptyset(&old_set) == -1) {
		printf("u fasync: fail to empty sig set\n");
		return -4;
	}

	if (sigaddset(&set, SIGIO) == -1) {
		printf("u fasync: fail to add sigio to set\n");
		return -5;
	}

	if (sigprocmask(SIG_UNBLOCK, &set, &old_set) == -1) {
		printf("u fasync: fail to unmask sigio\n");
		return -6;
	}

	fd = open("/dev/fasync0", O_RDWR | O_CLOEXEC);
	if (fd == -1) {
		printf("u fasync: fail to open /dev/fasync0\n");
		return -1;
	}

	ret = fcntl(fd, F_SETOWN, getpid());
	if (ret == -1) {
		printf("u fasync: fail to bind process\n");
		return -2;
	}

	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | FASYNC);
	if (ret == -1) {
		printf("u fasync: fail to set fasync\n");
		return -3;
	}
	
	while (1) {
		num++;
	}
}
