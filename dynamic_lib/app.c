#include <stdio.h>
#include "api_dl.h"

int main()
{
	int a = 1, b = 5, c = 0xffff;

	c = add(a, b);
	printf("a + b = %d\n", c);

	c = multi(a, b);
	printf("a * b = %d\n", c);

	return 0;
}
