#define _GNU_SOURCE
#include <pthread.h>

pthread_mutex_t mutexlock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
	int i;

	pthread_mutex_lock(&mutexlock);
	i++;
	pthread_mutex_unlock(&mutexlock);

	return 0;
}

/* gcc test.c -lpthread */
