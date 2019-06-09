#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <sys/mman.h>

/*
 * test in x86 with gcc -rdynamic test.c is OK.
 *
 * however, in aarch64, return of backtrace is alway 1.
 * use gcc -rdynamic -funwind-tables test.c to solve this problem.
 */

void fun_3(void);

void handler(int sig, siginfo_t *si, void *unused)
{
	fun_3();
}

void fun_3(void)
{
#define SIZE 10
	void *buffer[SIZE];
	char **strings;
	int n, i;

	n = backtrace(buffer, SIZE);

	strings = backtrace_symbols(buffer, n);

	for (i = 0; i < n; i++) {
		printf("%s\n", strings[i]);
	}

	free(strings);

	exit(EXIT_FAILURE);
}

void fun_2(void)
{
	fun_3();
}

void fun_1(void)
{
	fun_2();
}

void fun_0(void)
{
	fun_1();
}

int main()
{
	struct sigaction sa;
	char *buffer;
	int pagesize;

	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	sigaction(SIGSEGV, &sa, NULL);

	pagesize = sysconf(_SC_PAGE_SIZE);
	buffer = memalign(pagesize, 4 * pagesize);

	if (mprotect(buffer, pagesize, PROT_READ | PROT_WRITE) == -1)
		printf("fail to set mprotect\n");

	printf("write a in buffer a\n");
	*buffer = 'a';
	printf("write a in buffer b\n");
	sleep(2);
	printf("write a in buffer c\n");

	if (mprotect(buffer, pagesize, PROT_READ) == -1)
		printf("fail to set mprotect\n");

	*buffer = 'b';

	//fun_0();	

	exit(EXIT_SUCCESS);
}

