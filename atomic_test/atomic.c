#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdatomic.h>

#define TEST_ATOMIC_RUN_NUM 0x1000000

void test_no_atomic()
{
	unsigned long sum = 0;
	int i;

	for (i = 0; i < TEST_ATOMIC_RUN_NUM; i++) {
		sum += 1;	
	}
}

void test_atomic()
{
	unsigned long sum = 0;
	int i;
	atomic_int j = ATOMIC_VAR_INIT(0);
	atomic_int k = ATOMIC_VAR_INIT(0);

	for (i = 0; i < TEST_ATOMIC_RUN_NUM; i++) {
//		__sync_fetch_and_add(&sum, 1);
		atomic_load(&j);
		sum += 1;	
	}
}

void main()
{
	struct timeval tv1, tv2, tv3, tv4;	

	gettimeofday(&tv3, NULL);
	test_atomic();
	gettimeofday(&tv4, NULL);

	gettimeofday(&tv1, NULL);
	test_no_atomic();
	gettimeofday(&tv2, NULL);


	printf("tv_sec: %ld, tv_usec: %ld\n", tv1.tv_sec, tv1.tv_usec);
	printf("tv_sec: %ld, tv_usec: %ld\n", tv2.tv_sec, tv2.tv_usec);
	printf("tv_sec: %ld, tv_usec: %ld\n", tv3.tv_sec, tv3.tv_usec);
	printf("tv_sec: %ld, tv_usec: %ld\n", tv4.tv_sec, tv4.tv_usec);

	printf("run time for no atomic: %ld\n", tv2.tv_usec - tv1.tv_usec);
	printf("run time for atomic: %ld\n", tv4.tv_usec - tv3.tv_usec);
}
