#include <stdio.h>
#include "include/wd.h"

int main()
{
	char *name = "name a";

	name = wd_get_accel_name(NULL, 0);
	printf("a name = %p\n", name);

	name = "name b";
	name = wd_get_accel_name("/dev/hisi_zip-0", -1);
	printf("b name = %p\n", name);

	name = "name c";
	name = wd_get_accel_name("/dev/hisi_zip-0", 2);
	printf("c name = %p\n", name);

	name = "name d";
	name = wd_get_accel_name("/dev/hisi_zip", 1);
	printf("d name = %p\n", name);

	return 0;
}
