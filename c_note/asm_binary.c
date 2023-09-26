#include <stdio.h>

extern int add2(int);

int main()
{
	int a = 1, ret = 0;

	ret = add2(a);
	printf("ret is %d\n", ret);

	return 0;
}
