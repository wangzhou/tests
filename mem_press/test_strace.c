#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>

#define MEM_SIZE	(1024 * 1024 * 1024)
#define SLEEP_TIME	1

int main()
{
	void *p;
	int ret;
	struct timeval time;

	p = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		 MAP_ANONYMOUS, -1, 0);
	if (p == MAP_FAILED)
		exit(1);

	ret = madvise(p, MEM_SIZE, MADV_HUGEPAGE);
	if (ret)
		exit(1);

	munmap(p, MEM_SIZE);

	gettimeofday(&time, NULL);

	return 0;
}
