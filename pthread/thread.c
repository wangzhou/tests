#include  <stdio.h>
#include  <pthread.h>
#include  <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int a = 0;

void cleanup_fun(void *d)
{
}

void *thread_fun(void *data)
{
	int i = 0;
	
	pthread_cleanup_push(cleanup_fun, NULL);
	while(1) {
		printf("thread fun %d\n", i++);
		usleep(1000 * 100);
	}

	pthread_cleanup_pop(0);
	//pthread_exit((void *)12);
	return (void *)13;
}

int main()
{
	pthread_t t1, t2;
	void *t1_ret;

	pthread_create(&t1, NULL, thread_fun, NULL);

	sleep(2);

	pthread_cancel(t1);
	pthread_join(t1, &t1_ret);

	printf("t1 return: %ld\n", (unsigned long)t1_ret);

	return 0;
}
