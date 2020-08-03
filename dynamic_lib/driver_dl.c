#include "api_internal.h"

static int add(int a, int b)
{
	return a + b;	
}

static int multi(int a, int b)
{
	return a * b;	
}

struct api_driver example_driver = {
	.add = add,
	.multi = multi,
};

SET_DRIVER(example_driver);
