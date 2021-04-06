#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<malloc.h>
#include<stdlib.h>

struct msg {
	struct msg *m_next;
	unsigned long val;
};

struct msg *workq;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void *process_msg(void *data)
{
	struct msg *mp;

	while (1) {
		pthread_mutex_lock(&qlock);
		while (!workq)
			pthread_cond_wait(&qready, &qlock);
		mp = workq;
		workq = mp->m_next;
		pthread_mutex_unlock(&qlock);

		/* now process the message mp */
		printf("---> output msg val: 0x%lx\n", mp->val);
		free(mp);
	}
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
	//	sleep(2);
		printf("---> input msg val: 0x%lx\n", mp->val);

		pthread_mutex_unlock(&qlock);
		sleep(1);
		pthread_cond_signal(&qready);
	}
}

int main()
{
	pthread_t t1, t2;

	pthread_create(&t1, NULL, enqueue_msg, NULL);
	pthread_create(&t2, NULL, process_msg, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}
