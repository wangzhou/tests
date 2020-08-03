struct api_driver {
	int (*add)(int a, int b);
	int (*multi)(int a, int b);
};

void __set_driver(struct api_driver *drv);

#define SET_DRIVER(drv)							      \
extern const struct api_driver api_driver_##drv	__attribute__((alias(#drv))); \
static void __attribute__((constructor)) set_driver(void)		      \
{									      \
	__set_driver(&drv);						      \
}
