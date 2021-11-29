#include <stdio.h>
#include <math.h>
#include <numa.h>
#include "hello.h"
#include "./lib/add.h"

int main()
{
	int max = 5;

	printf(HELLO);
	printf("1 + 2 = %d\n", add(1, 2));
	printf("sqrt(5) = %f\n", sqrt(5));
	max = numa_max_node();
	printf("max node = %d\n", max);

	return 0;
}
