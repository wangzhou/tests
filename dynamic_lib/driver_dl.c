#include "api_internal.h"

static int add(int a, int b)
{
	return a + b;	
}

static int multi(int a, int b)
{
	return a * b;	
}

struct api_driver one_driver = {
	.add = add,
	.multi = multi,
};

SET_DRIVER(&one_driver);
