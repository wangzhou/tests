#define	_GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <sched.h>

#define	THREAD_NUM	4

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
long long a = 0, b = 0;
struct timeval start, end;
double time_val, pps;

void *add_a(void *m)
{
	while (1) {
		pthread_mutex_lock(&lock);
		a++;
		pthread_mutex_unlock(&lock);
	}

	return NULL;
}

int main()
{
	pthread_t t[THREAD_NUM];
	cpu_set_t cpuset;
	int i;

	gettimeofday(&start, NULL);

	for (i = 0; i < THREAD_NUM; i++) {
		pthread_create(&t[i], NULL, add_a, NULL);
	}

	for (i = 0; i < THREAD_NUM; i++) {
		CPU_ZERO(&cpuset);
		CPU_SET(i, &cpuset);
		pthread_setaffinity_np(t[i], sizeof(cpu_set_t), &cpuset);
	}

	sleep(5);

	gettimeofday(&end, NULL);

	b = a;
	time_val = (end.tv_sec - start.tv_sec) * 1000000 +
		   (end.tv_usec - start.tv_usec);
	pps = b / time_val * 1000000;

	printf("a is 0x%llx\n", b);
	printf("PPS of adding a is %f\n", pps);

	for (i = 0; i < THREAD_NUM; i++) {
		pthread_join(t[i], NULL);
	}

	return 0;
}
