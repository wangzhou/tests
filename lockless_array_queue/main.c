#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

#define Q_SIZE 100
#define P_THREAD_NUM 4
#define C_THREAD_NUM 4
#define PUSH_NUM_PER_THREAD 10000
#define TOTAL_NUM (PUSH_NUM_PER_THREAD * P_THREAD_NUM)

#define Q_POS(count) ((count) % Q_SIZE)

/*
 *  +---+---+---+---+---+---+
 *  |   | x | x | x |   |   |
 *  +---+---+---+---+---+---+
 *        ^           ^   ^
 *       read      commit write
 */
struct queue {
	int data[Q_SIZE];
	atomic_int read_pos;
	atomic_int write_pos;
	atomic_int commit_pos;
};

struct queue g_queue;
pthread_t pthreads[P_THREAD_NUM + C_THREAD_NUM];
atomic_int pop_num = ATOMIC_VAR_INIT(0);

static void init(void)
{
	g_queue.read_pos = ATOMIC_VAR_INIT(0);
	g_queue.write_pos = ATOMIC_VAR_INIT(0);
	g_queue.commit_pos = ATOMIC_VAR_INIT(0);
}

/* return false if q is full */
static bool push(struct queue *q, int data)
{
        int curr_write;
        int curr_read;

        do {
                curr_write = __atomic_load_n(&q->write_pos, __ATOMIC_SEQ_CST);
                curr_read = __atomic_load_n(&q->read_pos, __ATOMIC_SEQ_CST);
                
                if (Q_POS(curr_write + 1) == Q_POS(curr_read)) {
                        return false;
                }
        } while (!__atomic_compare_exchange_n(&q->write_pos, &curr_write,
		 Q_POS(curr_write + 1), false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST));

        q->data[curr_write] = data;

        while (!__atomic_compare_exchange_n(&q->commit_pos, &curr_write,
	       Q_POS(curr_write + 1), false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST));
        
        return true;
}

/* return false if q is empty */
static bool pop(struct queue *q, int *data)
{
        int curr_commit;
        int curr_read;
	int tmp;

	do {
		curr_read = __atomic_load_n(&q->read_pos, __ATOMIC_SEQ_CST);
		curr_commit = __atomic_load_n(&q->commit_pos, __ATOMIC_SEQ_CST);

		if (Q_POS(curr_read) == Q_POS(curr_commit)) {
			return false;
		}

		tmp = q->data[curr_read];
		if (__atomic_compare_exchange_n(&q->read_pos, &curr_read,
		    Q_POS(curr_read + 1), false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
			*data = tmp;
			return true;
		}

	} while (1);
}

void *push_queue(void *d)
{
	int i;

	for (i = 0; i < PUSH_NUM_PER_THREAD; i++) {
		while (!push(&g_queue, i));
		//printf(">> push data: %d\n", i);
	}
}

void *pop_queue(void *d)
{
	int i, ret;

	while (1) {
		if (pop(&g_queue, &ret)) {
			__atomic_fetch_add(&pop_num, 1, __ATOMIC_SEQ_CST);
			//printf("<< pop data: %d\n", ret);
		}

		if (pop_num == TOTAL_NUM) {
			return NULL;
		}
	}
}

int main()
{
	int i, p = 0;
	pthread_t tmp;

	init();

	for (i = 0; i < P_THREAD_NUM; i++) {
		pthread_create(&tmp, NULL, push_queue, NULL);
		pthreads[p++] = tmp;
	}

	for (i = 0; i < C_THREAD_NUM; i++) {
		pthread_create(&tmp, NULL, pop_queue, NULL);
		pthreads[p++] = tmp;
	}

	for (i = 0; i < P_THREAD_NUM + C_THREAD_NUM; i++) {
		pthread_join(pthreads[i], NULL);
	}

	return 0;
}
