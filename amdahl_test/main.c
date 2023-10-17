//gcc main.c -lpthread --static
//./a.out --pthread_num=4 --serial_ratio=20 --working_time=2
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <stdatomic.h>
#include <getopt.h>
#include <unistd.h>

#define	WORK_ARRAY_NUM 100
#define	WORK_ITER_NUM 1000
#define	WORK_POS(pos) ((pos) % WORK_ARRAY_NUM)

static int pthread_num = 1;
static int serial_ratio = 20; // 20%
static int working_time = 2;
static atomic_int work_num = ATOMIC_VAR_INIT(0);

struct work {
	unsigned long serial_handle_data;
	unsigned long para_handle_data;
};

struct work works[WORK_ARRAY_NUM];
pthread_spinlock_t lock = 0;
static atomic_int curr_work_pos = ATOMIC_VAR_INIT(0);

static void parse_input(int argc, char *argv[])
{

        int option_index = 0;
	int c;

        static struct option long_options[] = {
            {"pthread_num", required_argument, 0,  1},
            {"serial_ratio", required_argument, 0,  2},
            {"working_time", required_argument, 0,  3},
            {0, 0, 0, 0}
        };

	while (1) {
		c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 1:
			pthread_num = strtol(optarg, NULL, 0);
			break;
		case 2:
			serial_ratio = strtol(optarg, NULL, 0);
			break;
		case 3:
			working_time = strtol(optarg, NULL, 0);
			break;
		default:
			printf("bad input parameter, use: --pthread_num <num> --serial_ratio <ratio> --working_time <time>\n");
			exit(-1);
		}
	}
}

static unsigned long __do_work(unsigned long data, int ratio)
{
	unsigned long tmp;
	int i; 

	for (i = 0; i < WORK_ITER_NUM * ratio; i++) {
		tmp = data ^ 0x12345678;
		data = tmp % 0x123;
	}

	return data;
}

static void serial_do_work(struct work *work)
{
	work->serial_handle_data = __do_work(work->serial_handle_data, serial_ratio);
}

/* caculate para ratio from serial_ratio */
static void para_do_work(struct work *work)
{
	work->para_handle_data = __do_work(work->para_handle_data, 100 - serial_ratio);
}

void* do_work(void *data)
{
	atomic_int pos;

	while (1) {
		pos = __atomic_fetch_add(&curr_work_pos, 1, __ATOMIC_SEQ_CST);
		struct work *work = works + WORK_POS(pos);

		pthread_spin_lock(&lock);
		serial_do_work(work);
		pthread_spin_unlock(&lock);

		para_do_work(work);

		__atomic_fetch_add(&work_num, 1, __ATOMIC_SEQ_CST);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t *threads;
	int i;

	pthread_spin_init(&lock, PTHREAD_PROCESS_SHARED);
	parse_input(argc, argv);

	threads = malloc(sizeof(pthread_t) * pthread_num);
	assert(threads != NULL);

	for (i = 0; i < pthread_num; i++) {
		pthread_create(&threads[i], NULL, do_work, NULL);
	}

	sleep(working_time);

	printf("amdahl test result: pthread_num=%d, serial_ratio=%d, working_time=%d, work_num=%d\n",
	       pthread_num, serial_ratio, working_time, work_num);

	free(threads);
	return 0;
}
