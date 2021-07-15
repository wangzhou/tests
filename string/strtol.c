#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
	char *s = "123w23";
	int val = strtol(s, NULL, 10);

	printf("val: %d\n", val);

	return 0;
}
