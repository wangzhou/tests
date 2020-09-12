#include <stdio.h>

enum test {
	TEST1 = 0,
	TEST2,
	TEST3,
};

int add(int a, enum test b)
{
	return a + b;
}

int main()
{
	int c;

	c = add(1, 5);	
	printf("c: %d\n", c);
}
