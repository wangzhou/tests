#include <stdio.h>
#include <numa.h>

int main()
{
	int i = numa_max_node();

	printf("max node: %d\n", i);

	return 0;
}
