#include <stdio.h>

int add(int a, int b) __attribute__((deprecated));

int add(int a, int b)
{
	return a + b;
}

int main()
{
	int sum;

	sum = add(1, 2);
	printf("sum is %d\n", sum);

	return 0;
}
