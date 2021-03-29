#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>

#define MEM_SIZE	(1024 * 1024 * 1024)
#define SLEEP_TIME	1
#define PAGE_SIZE       (2 * 1024 * 1024)
#define MAP_LOOP	100000000
//#define MAP_LOOP	10

int main()
{
	struct timeval begin, end;
	unsigned long i, j;
	void *p, *tmp;
	float tc;
	int ret;

	gettimeofday(&begin, NULL);

	for (i = 0; i < MAP_LOOP; i++) {
		p = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
			 -1, 0);
		if (p == MAP_FAILED) {
			printf("---> error 1\n");
			exit(1);
		}

		ret = madvise(p, MEM_SIZE, MADV_HUGEPAGE);
		if (ret) {
			printf("---> error 2\n");
			exit(1);
		}

		tmp = p;
		for (j = 0; j < MEM_SIZE / PAGE_SIZE; j++) {
			memset(tmp, 3, 1);
			tmp += PAGE_SIZE;
			usleep(250);
		}

		munmap(p, MEM_SIZE);
	}

	gettimeofday(&end, NULL);

	tc = (float)((end.tv_sec - begin.tv_sec) * 1000000 + end.tv_usec - begin.tv_usec);
	printf("---> time is %0.3f s\n", tc / 1000000);

	return 0;
}
