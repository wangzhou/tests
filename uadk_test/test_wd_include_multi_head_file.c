#include <stdio.h>
#include "include/wd.h"
#include "include/wd_aead.h"
#include "include/wd_cipher.h"
#include "include/wd_digest.h"
#include "include/wd_comp.h"
#include "include/wd_rsa.h"
#include "include/wd_dh.h"

int main()
{
	struct uacce_dev_list *list;
	void *ret;

	list = wd_get_accel_list("zlib");
	if (!list)
		return -1;

	handle_t h_ctx = wd_request_ctx(list->dev);
	if (!h_ctx)
		return -1;

	ret = wd_drv_mmap_qfr(h_ctx, 2);
	printf("ret is %p\n", ret);

	wd_release_ctx(h_ctx);

	wd_free_list_accels(list);

	return 0;
}

/* gcc test_wd_drv_mmap_qfr.c -lwd -I./ -L.libs --static */
