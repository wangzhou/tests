#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
	char *s = "12345";
	char *t;
	char t1[] = {'1', '2', ' ', '3', '4', ' ', '5', '6', '\0'};
	char *str = t1;
	char *tmp;
	char *num = "1e3";
	long d;
	char *ret = NULL;
	char *ip = "127.2.3.4";
	char *ip_wrong = "127.2.3.4.";
	char *ip_wrong_1 = "127.....4";
	char buf[256];
	int i = 0;

	t = rindex(s, '3');
	printf("%s\n", t);

	while (tmp = strtok(str, " ")) {
		printf("%s\n", tmp);
		printf("---> %ld\n", strtol(tmp, NULL, 0));
		str = NULL;
	}

	d = strtol(num, &ret, 10);
	printf("--> %ld\n", d);
	printf("--> %s\n", ret);

	printf("-->test tail:\n");
	//strcpy(buf, ip_wrong);
	strcpy(buf, ip_wrong_1);
	//strcpy(buf, ip);

	tmp = strtok(buf, ".");
	if (tmp != NULL)
		printf("%s\n", tmp);

	while (1) {
		printf("---> %d\n", ++i);
		tmp = strtok(NULL, ".");
		//if(i == 2)
		//	assert(*tmp == '\0');
		if (tmp != NULL)
			printf("%s\n", tmp);
		else
			break;
	}

	return 0;
}
