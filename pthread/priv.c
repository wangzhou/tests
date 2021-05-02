#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

pthread_key_t key1;

void *thread_print_priv_val(void *data)
{
	pthread_t id;
	int *t;

	id = pthread_self();

	t = pthread_getspecific(key1);
	if (t)
	 	printf("thread_id: %lu: key1 -> value: %d\n", id, *t);
	else
	 	printf("thread_id: %lu: key1 -> value: NULL\n", id);
}

void *thread2(void *data)
{
	int *t, ret;

	t = malloc(10);
	if (!t)
		exit(-4);
	*t = 22;
	ret = pthread_setspecific(key1, t);
	if (ret)
		exit(-5);

	thread_print_priv_val(NULL);

	pthread_key_delete(key1);
}

int main()
{
	pthread_t t1, t2;
	void *p;
	int ret;

	printf("main thread: %lu\n", pthread_self());

	ret = pthread_key_create(&key1, free);
	if (ret)
		exit(-1);
	p = malloc(10);
	if (!p)
		exit(-2);
	*(int *)p = 66;
	ret = pthread_setspecific(key1, p);
	if (ret)
		exit(-3);

	thread_print_priv_val(NULL);
	pthread_create(&t1, NULL, thread_print_priv_val, NULL);
	printf("thread1: %lu\n", t1);

	pthread_create(&t2, NULL, thread2, NULL);
	printf("thread2: %lu\n", t2);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	pthread_key_delete(key1);

	return 0;
}
