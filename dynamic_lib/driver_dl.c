#include "api_internal.h"

int add(int a, int b)
{
	return a + b;	
}

int multi(int a, int b)
{
	return a * b;	
}

struct api_driver one_driver = {
	.add = add,
	.multi = multi,
};

SET_DRIVER(&one_driver);
