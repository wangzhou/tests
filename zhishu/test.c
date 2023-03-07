#include <stdio.h>
#include <math.h>
#include <stdbool.h>


bool is_zhishu(int a)
{
	int i, b;

	b = sqrt(a);
	
	for (i = 2; i <= b; i++) {
		if (a % i == 0) {
			return false;
		}
	}

	return true;
}

int main(void)
{
	int a = 0, i, j = 1, b = 0;
	int flag = 0;

	printf("桃桃，请输入两个数, 我会为你输出这两个数之间的所有质数: ");
	scanf("%d %d", &a, &b);

	for (i = a; i <=b; i++) {
		if (is_zhishu(i)) {
			flag = 1;
			printf("%d ", i);
			j++;
		}

		if (j % 21 == 0) {
			j = 1;
			printf("\n");
		}
	}
	
	if (!flag)
		printf("没有质数");

	printf("\n");
 
	return 0;
}

// gcc test.c -lm
