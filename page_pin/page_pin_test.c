#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>

/*
 * One process can only open /dev/page_pin one time. 
 * All thread in above process can pin/unpin va related pa by:
 * ioctl(fd, TEST_PIN/TEST_UNPIN, addr); addr is pointer of struct test_pin_address.
 *
 * addr and size in struct test_pin_address are pin/unpin address and size.
 */
struct test_pin_address {
	unsigned long addr;
	unsigned long size;
};

#define TEST_PIN		_IOW('W', 0, struct test_pin_address)
#define TEST_UNPIN		_IOW('W', 1, struct test_pin_address)

MODULE_LICENSE("Dual BSD/GPL");

struct cdev cdev;
struct class *class;
/* device's memory pool */
struct file_priv {
	struct xarray array;
};

int test_open(struct inode *inode, struct file *file)
{
	struct file_priv *p;

	/* to do: one process at most open dev one time */

	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;
        file->private_data = p;

	xa_init(&p->array);

        return 0;
}

int test_release(struct inode *inode, struct file *file)
{
	struct file_priv *p = file->private_data;

	/* todo: unpin page and remove from xarry */
	
	xa_destroy(&p->array);
	kfree(file->private_data);

        return 0;
}

static int test_pin_page(struct file_priv *priv, struct test_pin_address *addr)
{
	unsigned int flags = FOLL_FORCE;
	unsigned long first, last, nr_pages;
	struct page **pages;
	int ret;

	first = (addr->addr & PAGE_MASK) >> PAGE_SHIFT;
	last = ((addr->addr + addr->size - 1) & PAGE_MASK) >> PAGE_SHIFT;
	nr_pages = last - first + 1;

	pages = kmalloc_array(nr_pages, sizeof(struct page *), GFP_KERNEL);
	if (!pages)
		return -ENOMEM;

	ret = pin_user_pages(addr->addr & PAGE_MASK, nr_pages,
			     flags | FOLL_LONGTERM, pages, NULL);
	if (ret != nr_pages) {
		pr_err("Failed to pin page\n");
		goto free;
	}

	/* todo: Add pin pages into xarray */
	return 0;

free:
	kfree(pages);
	return ret;
}

static int test_unpin_page(struct file_priv *priv,
			   struct test_pin_address *addr)
{
	/* todo: check input is ok */
	
	/* unpin */

	/* remove from xarray */
}

static long test_unl_ioctl(struct file *filep, unsigned int cmd,
			   unsigned long arg)
{
	struct file_priv *p = filep->private_data;
	struct test_pin_address addr;

	if (copy_from_user(&addr, (void __user *)arg,
			   sizeof(struct test_pin_address)))
		return -EFAULT;

	/* todo: check input is ok */

	switch (cmd) {
	case TEST_PIN:
		return test_pin_page(p, &addr);

	case TEST_UNPIN:
		return test_unpin_page(p, &addr);

	default:
		return -EINVAL;
	}
}

struct file_operations test_fops = {
        .owner = THIS_MODULE,
        .open = test_open,
        .release = test_release,
	.unlocked_ioctl	= test_unl_ioctl,
};

static int __init page_pin_init(void)
{
        unsigned int firstminor = 0;
        dev_t dev_id;
        int err = 0;
        unsigned int count = 1;
        char *dev_name = "page_pin_test";

        /* alloc dev_id */
        err = alloc_chrdev_region(&dev_id, firstminor, count, dev_name);
        if (err < 0) {
                pr_err("page_pin: can not allocate a cdev\n");
                return err;
        }

        /* register cdev */
        cdev_init(&cdev, &test_fops);
        err = cdev_add(&cdev, dev_id, count);
        if (err < 0) {
                pr_err("page_pin: can not add a cdev to system\n");
                return err;
        }

        class = class_create(THIS_MODULE, dev_name);
        if (IS_ERR(class)) {
                pr_err("page_pin: fail to create device %s\n", dev_name);
                return -1;
        }

        device_create(class, NULL, dev_id, NULL, "page_pin" "%d", 0);

        return 0;
}

static void __exit page_pin_exit(void)
{
        dev_t dev_id = cdev.dev;

        device_destroy(class, dev_id);
        class_destroy(class);
        cdev_del(&cdev);
}

module_init(page_pin_init);
module_exit(page_pin_exit);

MODULE_AUTHOR("Sherlock");
MODULE_DESCRIPTION("The driver is for testing page pin");
