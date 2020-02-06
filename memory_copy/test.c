#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

/*
 * memory copy 1G byte data in no fault and fault cases
 */

#define TEST_MEM_SIZE (1024 * 4)

void main()
{
	struct timeval tv1, tv2, tv3, tv4;	
	volatile char *src1, *dts1, *src2, *dts2;
	int ret = 0;

	src1 = malloc(TEST_MEM_SIZE);
	if (!src1)
		return -1;

	dts1 = malloc(TEST_MEM_SIZE);
	if (!dts1) {
		goto free_src1;
		ret = -1;
	}

	src2 = malloc(TEST_MEM_SIZE);
	if (!src2) {
		goto free_dts1;
		ret = -1;
	}

	dts2 = malloc(TEST_MEM_SIZE);
	if (!dts2) {
		goto free_src2;
		ret = -1;
	}

	memset(src1, 5, TEST_MEM_SIZE);

	gettimeofday(&tv1, NULL);
	memcpy(dts1, src1, TEST_MEM_SIZE);
	gettimeofday(&tv2, NULL);

	memset(src2, 5, TEST_MEM_SIZE);
	memset(dts2, 6, TEST_MEM_SIZE);

	gettimeofday(&tv3, NULL);
	memcpy(dts2, src2, TEST_MEM_SIZE);
	gettimeofday(&tv4, NULL);

	printf("tv_sec: %ld, tv_usec: %ld\n", tv1.tv_sec, tv1.tv_usec);
	printf("tv_sec: %ld, tv_usec: %ld\n", tv2.tv_sec, tv2.tv_usec);
	printf("tv_sec: %ld, tv_usec: %ld\n", tv3.tv_sec, tv3.tv_usec);
	printf("tv_sec: %ld, tv_usec: %ld\n", tv4.tv_sec, tv4.tv_usec);

	printf("performance of test1: %ld us/1G\n", (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec));
	printf("performance of test2: %ld us/1G\n", (tv4.tv_sec - tv3.tv_sec) * 1000000 + (tv4.tv_usec - tv3.tv_usec));

	free(dts2);
free_src2:
	free(src2);
free_dts1:
	free(dts1);
free_src1:
	free(src1);
	
	return ret;
}
