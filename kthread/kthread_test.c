#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("Dual BSD/GPL");
static int case_num = 1;
module_param(case_num, int, S_IRUGO);

static int thread_fun(void *date)
{
	unsigned long i;

	while (1) {
		i++;
	}

	return 0;
}

/* test if kthread_stop can stop a thread right now */
static int test_kthread_stop_a_thread(void)
{
	struct task_struct *thread;

	thread = kthread_run(thread_fun, NULL, "kthread_test");
	if (IS_ERR(thread)) {
		pr_info("test: fail to create a kthread\n");
		return -EPERM;
	}

	msleep(400);

	kthread_stop(thread);

	return 0;
}

static int thread_hello(void *date)
{
	pr_info("hello world!\n");
	
	while (!kthread_should_stop())
		schedule();

	return 0;
}

/* test to find when to call thread_stop */
static int test_kthread_stop_righ_after_run(void)
{
	struct task_struct *thread;

	thread = kthread_run(thread_hello, NULL, "kthread_hello");
	if (IS_ERR(thread)) {
		pr_info("test: fail to create a kthread\n");
		return -EPERM;
	}

	kthread_stop(thread);

	return 0;
}

static int thread_loop_hello(void *date)
{
	int i = 0;

	while (!kthread_should_stop()) {
		pr_info("hello world: %d!\n", i++);
		msleep(1000);
	}

	pr_info("bye :)\n");

	return 0;
}

/* normal */
static int test_kthread_stop_normal(void)
{
	struct task_struct *thread;

	thread = kthread_run(thread_loop_hello, NULL, "kthread_loop_hello");
	if (IS_ERR(thread)) {
		pr_info("test: fail to create a kthread\n");
		return -EPERM;
	}
	
	msleep(4000);

	kthread_stop(thread);

	return 0;
}

static int __init kthread_init(void)
{
	switch (case_num) {
	case 1:
		test_kthread_stop_a_thread();
		break;
	case 2:
		test_kthread_stop_righ_after_run();
		break;
	case 3:
		test_kthread_stop_normal();
		break;
	default:		
		pr_err("no test case found!\n");
	}

        return 0;
}

static void __exit kthread_exit(void)
{
}

module_init(kthread_init);
module_exit(kthread_exit);

MODULE_AUTHOR("Sherlock");
MODULE_DESCRIPTION("The driver for kthread study");
