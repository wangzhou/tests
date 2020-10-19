#include <stdio.h>

int main()
{
	int num;

	printf("123456:%n\n", &num);
	printf("%*c %d\n", num, ' ', 123);
	printf("%*c %d\n", num, ' ', 456);

	return 3;
}

#if 0
输出：
xxx@kllp05:~/tests/c_note$ ./a.out 
123456:
        123
        456
#endif
