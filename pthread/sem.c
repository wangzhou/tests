#define	_GNU_SOURCE
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>
#include<stdlib.h>
#include<semaphore.h>
#include <sched.h>

struct msg {
	struct msg *m_next;
	unsigned long val;
};

struct msg *workq;

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;

void *process_msg(void *data)
{
	struct msg *mp;

	printf("---> process thread starts\n");
//	sleep(3);
	
	while (1) {
		if (sem_wait(&sem))
			exit(-3);

		pthread_mutex_lock(&qlock);

		mp = workq;
		workq = mp->m_next;

		pthread_mutex_unlock(&qlock);

		/* now process the message mp */
		printf("---> output msg val: 0x%lx\n", mp->val);
		free(mp);
	}
}

void dump_list(struct msg *mp)
{
	while (mp) {
		printf("0x%lx ", mp->val);
		mp = mp->m_next;
	}

	printf("\n");
}

void *enqueue_msg(void *data)
{
	struct msg *mp, *cur;

	while (1) {
		mp = calloc(1, sizeof(*mp));
		if (!mp)
			exit(-1);
		mp->val = (unsigned long)mp;

		pthread_mutex_lock(&qlock);

		cur = workq;
	
		if (!workq)
			workq = mp;
		else {
			while (cur->m_next)
				cur = cur->m_next;
			cur->m_next = mp;
		}
		//sleep(1);
		printf("---> input msg val: 0x%lx\n", mp->val);
		dump_list(workq);

		pthread_mutex_unlock(&qlock);

		sleep(1);
		if (sem_post(&sem))
			exit(-2);
	}
}

int main()
{
	pthread_t t1, t2;
	if (sem_init(&sem, 0, 0))
		exit(-4);
	cpu_set_t cpuset;

	pthread_create(&t1, NULL, enqueue_msg, NULL);
	pthread_create(&t2, NULL, process_msg, NULL);

	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset);
	pthread_setaffinity_np(t1, sizeof(cpu_set_t), &cpuset);

	CPU_ZERO(&cpuset);
	CPU_SET(1, &cpuset);
	pthread_setaffinity_np(t2, sizeof(cpu_set_t), &cpuset);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	sem_destroy(&sem);

	return 0;
}
