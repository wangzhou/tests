#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/time.h>
#include <getopt.h>
#include <unistd.h>
#include <asm/types.h>
#include <assert.h>
#include <sched.h>
#include <pthread.h>

struct self_spinlock {
	__u32 lock;
};

struct test_option {
	/* 0: self spinlock; 1: pthread mutex lock */
	__u8 mode;
	__u16 thread_num;
	long long total;
};

struct thread_data {
	pthread_t p;
	struct timeval begin;
	struct timeval end;
	struct test_option *opt;
	long long *number;
};

typedef void *(*thread_fun)(void *);

long long number __attribute__((aligned(64))) = 0;
long long number_2 __attribute__((aligned(64))) = 0;
//long long number = 0;
//long long number_2 = 0;
struct self_spinlock spinlock;
pthread_mutex_t mutexlock = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t p_spinlock = 0;

static inline void self_spinlock(struct self_spinlock *lock)
{
	while (__atomic_test_and_set(&lock->lock, __ATOMIC_ACQUIRE))
		while (__atomic_load_n(&lock->lock, __ATOMIC_RELAXED))
			;
}

static inline void self_unspinlock(struct self_spinlock *lock)
{
	__atomic_clear(&lock->lock, __ATOMIC_RELEASE);
}

static void parse_cmd_line(int argc, char *argv[], struct test_option *opt)
{
        int option_index = 0;
	int c;

        static struct option long_options[] = {
            {"mode", required_argument, 0,  1},
            {"thread_num", required_argument, 0,  2},
            {"total", required_argument, 0,  3},
            {0, 0, 0, 0}
        };

	while (1) {
		c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 1:
			opt->mode = strtol(optarg, NULL, 0);
			break;
		case 2:
			opt->thread_num = strtol(optarg, NULL, 0);
			break;
		case 3:
			opt->total = strtol(optarg, NULL, 0);
			break;
		default:
			printf("bad input parameter, exit\n");
			exit(-1);
		}
	}
}

static void *test_thread_spinlock(void *data)
{	
	struct thread_data *t_date = (struct thread_data *)data;
	struct test_option *opt = t_date->opt;
	long long total = opt->total;

	gettimeofday(&t_date->begin, NULL);

	while (1) {
		self_spinlock(&spinlock);

		if (*(t_date->number) == total) {
			self_unspinlock(&spinlock);
			break;
		}

		(*(t_date->number))++;

		self_unspinlock(&spinlock);
	}

	gettimeofday(&t_date->end, NULL);
}

static void *test_thread_mutexlock(void *data)
{	
	struct thread_data *t_date = (struct thread_data *)data;
	struct test_option *opt = t_date->opt;
	long long total = opt->total;
	
	gettimeofday(&t_date->begin, NULL);

	while (1) {
		pthread_mutex_lock(&mutexlock);

		if (*(t_date->number) == total) {
			pthread_mutex_unlock(&mutexlock);
			break;
		}

		(*(t_date->number))++;

		pthread_mutex_unlock(&mutexlock);
	}

	gettimeofday(&t_date->end, NULL);
}

static void *test_pthread_spinlock(void *data)
{
	struct thread_data *t_date = (struct thread_data *)data;
	struct test_option *opt = t_date->opt;
	long long total = opt->total;

	gettimeofday(&t_date->begin, NULL);

	while (1) {
		pthread_spin_lock(&p_spinlock);

		if (*(t_date->number) == total) {
			pthread_spin_unlock(&p_spinlock);
			break;
		}

		(*(t_date->number))++;

		pthread_spin_unlock(&p_spinlock);
	}

	gettimeofday(&t_date->end, NULL);
}

int main(int argc, char *argv[])
{
	struct thread_data *thread_data_array, *tmp;
	struct test_option opt = {0};
	long long time = 0, time_ops;
	thread_fun fun;
	cpu_set_t mask;
	struct timeval begin;
	struct timeval end;
	float ops;
	int i;

	parse_cmd_line(argc, argv, &opt);

	printf("mode: %d, thread_num: %d, total: %lld\n",
	       opt.mode, opt.thread_num, opt.total);
	
	thread_data_array = malloc(sizeof(struct thread_data) * opt.thread_num);
	if (!thread_data_array)
		return -1;

	/* prepare test */
	if (opt.mode == 0)
		fun = test_thread_spinlock;
	else if (opt.mode == 1)
		fun = test_thread_mutexlock;
	else {
		pthread_spin_init(&p_spinlock, PTHREAD_PROCESS_SHARED);
		fun = test_pthread_spinlock;
	}

	for (i = 0; i < opt.thread_num; i++) {
		thread_data_array[i].opt = &opt;
		/* you can add different value for different threads */
		thread_data_array[i].number = &number;
	}

	gettimeofday(&begin, NULL);

	/* start test */
	for (i = 0; i < opt.thread_num; i++) {
		/* fix me: bind to different core */
		pthread_create(&thread_data_array[i].p, NULL, fun,
			       thread_data_array + i);

		CPU_ZERO(&mask);
		CPU_SET(i + 1, &mask);

		if (pthread_setaffinity_np(thread_data_array[i].p, sizeof(mask),
					   &mask) < 0)
			printf("fail to bind thread to cpu %d\n", i + 1);
	}

	for (i = 0; i < opt.thread_num; i++) {
		pthread_join(thread_data_array[i].p, NULL);
	}

	gettimeofday(&end, NULL);
	time_ops = ((end.tv_sec - begin.tv_sec) * 1000000 +
		    (end.tv_usec - begin.tv_usec));
	ops = (float)opt.total / opt.thread_num / time_ops * 1000000 / (1024 * 1024);
	printf("ops is %f M/s\n", ops);

	/* get test result */
	for (i = 0; i < opt.thread_num; i++) {
		tmp = &thread_data_array[i];
		time += ((tmp->end.tv_sec - tmp->begin.tv_sec) * 1000000 +
			 (tmp->end.tv_usec - tmp->begin.tv_usec));
	}

	printf("total time is %lld us\n", time);

	return 0;
}

/* gcc test.c -lpthread */
