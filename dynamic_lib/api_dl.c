#include <stdio.h>
#include <dlfcn.h>
#include "api_internal.h"

extern const struct api_driver api_driver_example_driver;
const struct api_driver *global_driver = NULL;

int add(int a, int b)
{
	return global_driver->add(a, b);
}

int multi(int a, int b)
{
	return global_driver->multi(a, b);
}

void __set_driver(struct api_driver *drv)
{
	global_driver = drv;
}

#ifdef STATIC_DRV
void api_driver_init(void)
{
	/* a parameter can be introduced to decide to choose specific driver */
	global_driver = &api_driver_example_driver;
}
#else
static void __attribute__((constructor)) open_driver_dl(void)
{
	void *driver_dl;

	driver_dl = dlopen("./libdriver.so", RTLD_NOW);
	if (!driver_dl)
		printf("Fail to open libdriver\n");
}
#endif

