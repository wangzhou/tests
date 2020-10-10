#include <stdio.h>
#include "include/wd.h"

int main()
{
	int numa_id = 0xffff;

	handle_t h_ctx = wd_request_ctx("/dev/hisi_zip-1");
	if (!h_ctx)
		return -1;

	numa_id = wd_get_numa_id(h_ctx);
	printf("numa id is %d\n", numa_id);

	wd_release_ctx(h_ctx);

	return 0;
}

/* gcc test_wd_get_numa_id.c -lwd -I./ -L.libs --static */
