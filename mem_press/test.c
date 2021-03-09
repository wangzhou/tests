#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#define MEM_SIZE	(1024 * 1024 * 1024)
#define SLEEP_TIME	1

int main()
{
	void *p;
	unsigned long i;
	int ret;
#if 0
	for (i = 0; i < 100000000; i++) {
		p = malloc(MEM_SIZE);
		if (!p)
			exit(1);

		memset(p, 3, MEM_SIZE);
//		usleep(1000000);

		free(p);
	}
#endif
//#if 0
//	for (i = 0; i < 100000000; i++) {
		p = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
			 -1, 0);
		if (p == MAP_FAILED) {
			printf("---> error 1\n");
			exit(1);
		}
#if 0
		ret = madvise(p, MEM_SIZE, MADV_HUGEPAGE);
		if (ret) {
			printf("---> error 2\n");
			exit(1);
		}
#endif
	for (i = 0; i < 100000000; i++)
		memset(p, 3, MEM_SIZE);

		munmap(p, MEM_SIZE);
//	}
//#endif
	return 0;
}
