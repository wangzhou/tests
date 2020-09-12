#include <malloc.h>
#include <stdio.h>

#define SIZE (1024 * 1024)

void *array[SIZE];

int main()
{
	int i;

	for (i = 0; i < SIZE; i++) {
		array[i] = malloc(1024 * 3);
	}

	while (1);

	return 0;
}
