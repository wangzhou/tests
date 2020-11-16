#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *s = "12345";
	char *t;
	char t1[] = {'1', '2', ' ', '3', '4', ' ', '5', '6', '\0'};
	char *str = t1;
	char *tmp;

	t = rindex(s, '3');
	printf("%s\n", t);

	while (tmp = strtok(str, " ")) {
		printf("%s\n", tmp);
		printf("---> %ld\n", strtol(tmp, NULL, 0));
		str = NULL;
	}

	return 0;
}
