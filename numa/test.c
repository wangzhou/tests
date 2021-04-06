#include <stdio.h>
#include <numa.h>

int main()
{
	int i = numa_max_node();
	int nodes[i + 1];

	printf("max node: %d\n", i);

	return 0;
}
