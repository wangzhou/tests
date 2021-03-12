#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>

#define MEM_SIZE	(1024 * 1024 * 1024)
#define TEST_TIME	100

int main()
{
	void *p;
	unsigned long i;
	int ret;
	struct timeval begin, end;
	float tc, speed;

	p = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
		 -1, 0);
	if (p == MAP_FAILED) {
		printf("---> error 1\n");
		exit(1);
	}

	gettimeofday(&begin, NULL);

	for (i = 0; i < TEST_TIME; i++)
		memset(p, 3, MEM_SIZE);

	gettimeofday(&end, NULL);

	tc = (float)((end.tv_sec - begin.tv_sec) * 1000000 + end.tv_usec - begin.tv_usec);
	speed = (unsigned long)TEST_TIME * MEM_SIZE / tc / 1024 / 1024 * 1000000;

	printf("---> speed is %0.3f MB/s\n", speed);
	printf("---> time is %0.3f s\n", tc / 1000000);

	munmap(p, MEM_SIZE);

	return 0;
}
