#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/time.h>
#include <getopt.h>
#include <unistd.h>
#include <asm/types.h>
#include <assert.h>

struct wd_lock {
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
};

typedef void *(*thread_fun)(void *);

long long number = 0;
struct wd_lock spinlock;
pthread_mutex_t mutexlock = PTHREAD_MUTEX_INITIALIZER;

static inline void wd_spinlock(struct wd_lock *lock)
{
	while (__atomic_test_and_set(&lock->lock, __ATOMIC_ACQUIRE))
		while (__atomic_load_n(&lock->lock, __ATOMIC_RELAXED))
			;
}

static inline void wd_unspinlock(struct wd_lock *lock)
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
		wd_spinlock(&spinlock);

		if (number == total) {
			wd_unspinlock(&spinlock);
			break;
		}

		number++;

		wd_unspinlock(&spinlock);
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

		if (number == total) {
			pthread_mutex_unlock(&mutexlock);
			break;
		}

		number++;

		pthread_mutex_unlock(&mutexlock);
	}

	gettimeofday(&t_date->end, NULL);
}

int main(int argc, char *argv[])
{
	struct thread_data *thread_data_array, *tmp;
	struct test_option opt = {0};
	long long time = 0;
	thread_fun fun;
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
	else
		fun = test_thread_mutexlock;

	for (i = 0; i < opt.thread_num; i++)
		thread_data_array[i].opt = &opt;

	/* start test */
	for (i = 0; i < opt.thread_num; i++) {
		/* fix me: bind to different core */
		pthread_create(&thread_data_array[i].p, NULL, fun,
			       thread_data_array + i);
	}

	for (i = 0; i < opt.thread_num; i++) {
		pthread_join(thread_data_array[i].p, NULL);
	}

	/* get test result */
	for (i = 0; i < opt.thread_num; i++) {
		tmp = &thread_data_array[i];
		time += ((tmp->end.tv_sec - tmp->begin.tv_sec) * 1000000 +
			 (tmp->end.tv_usec - tmp->begin.tv_usec));
	}


	printf("total time is %lld us for %s\n", time, opt.mode ? "mutexlock" : "spinlock");

	return 0;
}

/* gcc test.c -lpthread */
