#include <stdio.h>
#include <dlfcn.h>
#include "api_internal.h"

struct api_driver *global_driver;

void __set_driver(struct api_driver *drv)
{
	global_driver = drv;
}

int add(int a, int b)
{
	return global_driver->add(a, b);
}

int multi(int a, int b)
{
	return global_driver->multi(a, b);
}

static void __attribute__((constructor)) open_driver_dl(void)
{
	void *driver_dl;

	driver_dl = dlopen("./libdriver.so", RTLD_LAZY);
	if (!driver_dl)
		printf("Fail to open libdriver\n");
}
