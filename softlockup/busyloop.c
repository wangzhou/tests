#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cleanup.h>

MODULE_LICENSE("Dual BSD/GPL");

static int __init busyloop_init(void)
{
	spinlock_t lock;
	spinlock_t s_lock;
	int tmp, tmp1, tmp2;

	spin_lock_init(&lock);

	guard(spinlock_irq)(&lock);
	while (1) {
		scoped_guard(spinlock_irq, &s_lock) {
			tmp++;
			tmp1++;
		}
		tmp2++;
	}

        return 0;
}

static void __exit busyloop_exit(void) {}

module_init(busyloop_init);
module_exit(busyloop_exit);

MODULE_AUTHOR("Sherlock");
MODULE_DESCRIPTION("The driver is for testing soft lockup");

//make -C /home/wangzhou/linux-kernel-warpdrive M=/home/wangzhou/tests/busyloop modules
