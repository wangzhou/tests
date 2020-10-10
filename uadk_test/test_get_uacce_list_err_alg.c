#include <stdio.h>
#include "include/wd.h"

int main()
{
	struct uacce_dev_list *list;

	list = wd_get_accel_list("zip");
	printf("list is %p\n", list);

	return 0;
}

/* gcc test_wd_drv_mmap_qfr.c -lwd -I./ -L.libs --static */
