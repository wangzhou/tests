#include <stdio.h>
#include <malloc.h>

int main()
{
	int **array;	
	int i, j;
	int k = 1;

	/* 4 * 3 int array */
	array = calloc(4, sizeof(int *));
	if (!array)
		return -1;
	
	for (i = 0; i < 4; i++) {
		array[i] = malloc(3 * sizeof(int));
		if (!array[i])
			return -1;
	}

	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
			array[i][j] = k++;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
			printf("%d ", array[i][j]);
	printf("\n");

	return 0;
}
