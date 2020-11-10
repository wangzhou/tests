#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>

int main()
{
	int len = 128 * 1024;
	unsigned long *p;
	int i;

	p = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE |
		 MAP_ANONYMOUS | MAP_HUGETLB | (16 << 26), -1, 0);
	if (p == MAP_FAILED) {
		perror("fail to allocate huge page\n");
		return -1;
	}

	for (i = 0; i < len / sizeof(*p); i++) {
		*(p + i) = 20;
	}

	while (1);
}
