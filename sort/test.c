#include <stdlib.h>
#include <stdio.h>

int comp(const void *a, const void *b)
{
	const int *a_in = a;
	const int *b_in = b;

	if (*a_in < *b_in)
		return -1;
	if (*a_in == *b_in)
		return 0;
	if (*a_in > *b_in)
		return 1;
}

int main()
{
	int i;

	int array[5] = {3, 4, 2, 7, 1};

	qsort(array, 5, sizeof(int), comp);

	for (i = 0; i < 5; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");

	return 0;
}
