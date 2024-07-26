#include <stdio.h>

void print(int *t)
{
	printf("cleanup: %d\n", *t);
}

void do_something(void)
{
	printf("do something\n");
}

int main()
{
	printf("main function start\n");
	int a __attribute__((__cleanup__(print))) = 10;

	for (int i = 0; i < 3;  i++) {
		int b __attribute__((__cleanup__(print))) = i;
		do_something();
	}

	return 0;
}
