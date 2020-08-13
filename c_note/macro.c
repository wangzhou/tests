#include <stdio.h>

#define TEST(a, b) a##b
#define STR(a) #a 
#define A 1000
#define ADD(a, b) ((a) + (b))
#define _TEST(a, b) TEST(a, b)

#define _STR(a) STR(a)

int main()
{
	printf("test 1: %d\n", TEST(1, 5));
	printf("test 2: %s\n", STR(just_test));
	printf("test 3: %d\n", ADD(A, A));

	/* test case 4 */
	printf("test 4: %s\n", STR(TEST(1, 9)));

	/* test case 5 */
	printf("test 5: %s\n", _STR(TEST(1, 9)));

	return 0;
}
