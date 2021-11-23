#include <stdio.h>
#include "hello.h"
#include "./lib/add.h"

int main()
{
	printf(HELLO);
	printf("1 + 2 = %d\n", add(1, 2));

	return 0;
}
