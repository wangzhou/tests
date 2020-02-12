#include <stdio.h>

int main(int ac, char *av[])
{
	char *test1 = "string1 string2 string3";
	char *test2 = " string1    string2   string3";
	char *test3 = " string1    g   string3";
	char str1[10], str2[10], str3[10];
	char c;
	
//	sscanf(test1, "%s%s%s", str1, str2, str3);
//	sscanf(test2, "%s%s%s", str1, str2, str3);
	/* why we can not input 'g'? */
	sscanf(test3, "%s%[a-z]%s", str1, &c, str3);
	printf("%s\n", str1);
//	printf("%s\n", str2);
	printf("%c\n", c);
	printf("%s\n", str3);

	return 0;
}
