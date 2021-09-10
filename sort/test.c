#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int array2[3][4] = {{6, 4, 2},
		    {7, 1, 3},
		    {8, 2, 1}};

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

int comp0(const void *a, const void *b)
{
	const int *a_in = a;
	const int *b_in = b;

	return *a_in - *b_in;
}

int comp1(const void *a, const void *b)
{
	const int *a_in = a;
	const int *b_in = b;

	return *(a_in + 1) - *(b_in + 1);
}

int comp2(const void *a, const void *b)
{
	const int **a_in = a;
	const int **b_in = b;

	return *(*a_in + 2) - *(*b_in + 2);
}

int main()
{
	int i, j;

	int array[5] = {3, 4, 2, 7, 1};

	int array2[3][3] = {{6, 4, 3},
		    	    {7, 1, 2},
		    	    {8, 2, 1}};
	int **array3;

	qsort(array, 5, sizeof(int), comp);

	for (i = 0; i < 5; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");

	printf("array2 init:\n");
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("%d ", array2[i][j]);
		printf("\n");
	}

	qsort(array2, 3, sizeof(int) * 3, comp1);

	printf("array2 sort:\n");
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("%d ", array2[i][j]);
		printf("\n");
	}
	printf("\n");

	array3 = calloc(1, sizeof(int *) * 3);
	assert(array3);

	array3[0] = calloc(1, sizeof(int) * 3);
	array3[1] = calloc(1, sizeof(int) * 3);
	array3[2] = calloc(1, sizeof(int) * 3);
	assert(array3[0] && array3[1] && array3[2]);

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			array3[i][j] = array2[i][j];

	printf("array3 init:\n");
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("%d ", array3[i][j]);
		printf("\n");
	}

	qsort(array3, 3, sizeof(int *), comp2);

	printf("array3 sort:\n");
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("%d ", array3[i][j]);
		printf("\n");
	}
	printf("\n");

	return 0;
}
