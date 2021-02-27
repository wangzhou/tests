#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MEM_SIZE	(1024 * 1024 * 1024)
#define SLEEP_TIME	1

int main()
{
	void *p;
	unsigned long i;

	for (i = 0; i < 100000000; i++) {
		p = malloc(MEM_SIZE);
		if (!p)
			exit(1);

		memset(p, 3, MEM_SIZE);
//		usleep(1000000);

		free(p);
	}

	return 0;
}
