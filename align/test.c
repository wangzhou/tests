#include <stdio.h>

int main()
{
	struct test {
		double h;
		char a;
	} test_a;

	/* result is 16 */
	printf("size of test_a: %d\n", sizeof(test_a));

	return 0;
}
