#include <string.h>
#include <stdio.h>

int main()
{
	char *s = "12345";
	char *t;

	t = rindex(s, '3');
	printf("%s\n", t);

	return 0;
}
