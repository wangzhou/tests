#include <stdio.h>

int main()
{
	int a = 0;

	__atomic_fetch_add(&a, 2, __ATOMIC_ACQUIRE);

	printf("---> add: %d\n", a);

	__atomic_fetch_sub(&a, 1, __ATOMIC_ACQUIRE);

	printf("---> sub: %d\n", a);
}
