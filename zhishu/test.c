#include <stdio.h>
#include <math.h>

int main(void)
{
	int a = 0, b, i;

	printf("桃桃，请输入一个数: ");
	scanf("%d", &a);

	b = sqrt(a);
	
	printf("b是：%d\n", b);

	for (i = 2; i <= b; i++) {
		printf("检测：%d 除以 %d\n", a, i);
		if (a % i == 0) {
			printf("%d 不是质数！可以被%d整除\n", a, i);
			return 0;
		}
	}

	printf("%d 是质数！\n", a);

	return 0;
}

// gcc test.c -lm
