#define _GNU_SOURCE
#include <pthread.h>

pthread_spinlock_t p_spinlock = 0;

int main(int argc, char *argv[])
{
	int i;

	pthread_spin_init(&p_spinlock, PTHREAD_PROCESS_SHARED);

	//pthread_spin_lock(&p_spinlock);
	i++;
	pthread_spin_unlock(&p_spinlock);

	return 0;
}

/* gcc test.c -lpthread */
