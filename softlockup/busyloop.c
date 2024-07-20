#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int __init busyloop_init(void)
{
	spinlock_t lock;
	int tmp;

	spin_lock_init(&lock);

	spin_lock_irq(&lock);
	while (1) {
		tmp++;
	}
	spin_unlock_irq(&lock);

        return 0;
}

static void __exit busyloop_exit(void) {}

module_init(busyloop_init);
module_exit(busyloop_exit);

MODULE_AUTHOR("Sherlock");
MODULE_DESCRIPTION("The driver is for testing soft lockup");

//make -C /home/wangzhou/linux-kernel-warpdrive M=/home/wangzhou/tests/busyloop modules
