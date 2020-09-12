#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/time.h>

struct wd_lock {
	__u32 lock;
};

struct option {
	/* 0: self spinlock; 1: pthread mutex lock */
	__u8 mode;
	__u16 thread_num;
	long long total;
};

typedef void *(*thread_fun)(void *);

long long number;
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

static void parse_cmd_line(int argc, char *argv[], struct option *opt)
{
        int option_index = 0;
	int c;

        static struct option long_options[] = {
            {"mode", required_argument, 0,  0},
            {"thread_num", required_argument, 0,  0},
            {"total", required_argument, 0,  0},
            {0, 0, 0, 0}
        };

	while (1) {
		c = getopt_long(argc, argv, NULL, long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			opt->mode = xxx;
		case 0:
			opt->thread_num = xxx;
		case 0:
			opt->total = xxx;
		default:
		}
	}
}

static void *test_thread_spinlock(void *data)
{	
	struct option *opt = (struct option *)data;
	long long total = opt->total;

	while (number != total) {
		wd_spinlock(&spinlock);
		number++;
		wd_unspinlock(&spinlock);
	}
}

static void *test_thread_mutexlock(void *data)
{	
	struct option *opt = (struct option *)data;
	long long total = opt->total;

	while (number != total) {
		pthread_mutex_lock(&mutexlock);
		number++;
		pthread_mutex_unlock(&mutexlock);
	}
}

int main(int argc, char *argv[])
{
	pthread_t *test_thread_array;
	struct timeval begin, end;
	struct option opt;
	long long time;
	int i;

	parse_cmd_line(argc, argv, &opt);

	test_thread_array = malloc(sizeof(pthread_t) * opt->thread_num);
	if (!test_thread_array)
		return -1;

	if (opt->mode == 0)
		thread_fun = test_thread_spinlock;
	else
		thread_fun = test_thread_mutexlock;

	gettimeofday(&begin, NULL);

	for (i = 0; i < opt->thread_num; i++) {
		/* fix me: bind to different core */
		pthread_create(&test_thread_array[i], NULL, thread_fun, &opt);
	}

	for (i = 0; i < opt->thread_num; i++) {
		pthread_join(test_thread_array[i], NULL);
	}

	gettimeofday(&end, NULL);

	time = ((end.tv_sec - begin.tv_sec) * 1000000 +
	       (end.tv_usec - begin.tv_usec));

	printf("time is %lld us for %s\n", time, opt->mode ? "mutexlock" : "spinlock");

	return 0;
}
