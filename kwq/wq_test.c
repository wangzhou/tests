#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/delay.h>

MODULE_LICENSE("Dual BSD/GPL");

struct work_struct work0, work1, work2, work3, work4, work5, work6, work7, work8;
struct workqueue_struct *test_wq, *ub_wq1, *ub_wq2, *ub_wq3, *ub_wq4;

#define WORK_FUN(n)						\
static void work_fun##n(struct work_struct *work)		\
{								\
	int num = n;						\
								\
	pr_info("I am in CPU%d in PID%d[%s] -- %d\n",		\
			raw_smp_processor_id(),			\
			current->pid, current->comm, num);	\
}
//	msleep(5000);						
/* note: msleep above only be used in case 3. in other cases, comment out */

WORK_FUN(0)
WORK_FUN(1)
WORK_FUN(2)
WORK_FUN(3)
WORK_FUN(4)
WORK_FUN(5)
WORK_FUN(6)
WORK_FUN(7)
WORK_FUN(8)

static int __init wq_init(void)
{
	/*
	 * 1. unbound wq, queue_work: how to define on which cpu to run?
	 *    can we bind which cpu to run by queue_work_on? 
	 *
	 *    seems queue_work_on can not put a work on a cpu precisely!
	 *    seems queue_work_on can put a work to a cpu in a numa node.
	 *
	 *    seems queue_work put a work to current numa node's cpu.
	 *
	 * 2. per cpu wq: how to queue work, which API to use? 
	 *
	 *    schedule_work can queue work to current cpu's normal per cpu:
         *
	 *    taskset -c 7 insmod wq_test.ko
	 *    [ 6690.409802] before queue_work: I am in CPU7 in PID1735[insmod]
	 *    [ 6691.444386] I am in CPU7 in PID90[kworker/7:1]
	 *    [ 6692.468339] I am in CPU7 in PID90[kworker/7:1]
	 *    [ 6693.492337] I am in CPU7 in PID90[kworker/7:1]
	 *
	 *    how to put work to cpu's high pri per cpu??
	 *
	 * 3. unbound wq, in which case new kthread will be added to system?
	 *    e.g. in current mainline zip driver, one qp allocate one unbound
	 *         queue, seems one unbound queue will add one kthread?
	 *   
	 *    one new unbound queue will not surely add on kthread, one new
	 *    unbound queue will firstly find an already same work pool, if
	 *    we have a same work pool(param: nice, cpumask, no numa), use it.
	 *    if we do not have, create a new work pool, which will surely add
	 *    a new kthread.
	 *
	 *   if using an old work pool, it may add new kthread, only all workers
	 *   in this pool are blocked, it will add a new kthread.
	 *
	 * 4. can we alloc a bound work queue? what is difference with system
	 *    work queue.
	 *
	 * 5. add WQ_SYSFS, it can register related wq to sysfs.
	 */
	test_wq = alloc_workqueue("test_wq", WQ_UNBOUND | WQ_HIGHPRI | WQ_SYSFS, 0);
	//test_wq = alloc_workqueue("test_wq", WQ_UNBOUND | WQ_SYSFS, 0);
	if (!test_wq)
		return -1;

	INIT_WORK(&work0, work_fun0);
	INIT_WORK(&work1, work_fun1);
	INIT_WORK(&work2, work_fun2);
	INIT_WORK(&work3, work_fun3);
	INIT_WORK(&work4, work_fun4);
	INIT_WORK(&work5, work_fun5);
	INIT_WORK(&work6, work_fun6);
	INIT_WORK(&work7, work_fun7);
	INIT_WORK(&work8, work_fun8);

	pr_info("before queue_work: I am in CPU%d in PID%d[%s]\n",
		raw_smp_processor_id(), current->pid, current->comm);

	/* case 1 */
	/* can not run in cpu 5 everytime, but always run in cpu of same numa node */
//	queue_work_on(5, test_wq, &work1);

	/* case 2 */
//	schedule_work(&work1);

	/* case 3: let's put many works to one wq to see if new worker created */
/*
	queue_work(test_wq, &work0);
	queue_work(test_wq, &work1);
	queue_work(test_wq, &work2);
	queue_work(test_wq, &work3);
	queue_work(test_wq, &work4);
	queue_work(test_wq, &work5);
	queue_work(test_wq, &work6);
	queue_work(test_wq, &work7);
	queue_work(test_wq, &work8);
*/
	/* case 4 */
/*
	schedule_work(&work0);
	schedule_work(&work1);
	schedule_work(&work2);
	schedule_work(&work3);
	schedule_work(&work4);
	schedule_work(&work5);
	schedule_work(&work6);
	schedule_work(&work7);
	schedule_work(&work8);
*/
	/* case 5 */
	/* this case shows that it can put works in cpu5 always */
/*
	schedule_work_on(5, &work0);
	schedule_work_on(5, &work1);
	schedule_work_on(5, &work2);
	schedule_work_on(5, &work3);
	schedule_work_on(5, &work4);
	schedule_work_on(5, &work5);
	schedule_work_on(5, &work6);
	schedule_work_on(5, &work7);
	schedule_work_on(5, &work8);
*/
	/* case 6 */
	/*
	 * will put below different worker in same unbound pool, however, may
	 * put works in different worker event without "sleep", I do not know
	 * why??
	 *
	 * however, if we queue_work_on same work to same cpu and wq, it will
	 * alway on one worker.(note: add sleep so that all work1 can be put
	 * into wq)
	 */
	ub_wq1 = alloc_workqueue("ub_wq1", WQ_UNBOUND | WQ_HIGHPRI | WQ_CPU_INTENSIVE | WQ_SYSFS, 0);
	ub_wq2 = alloc_workqueue("ub_wq2", WQ_UNBOUND | WQ_HIGHPRI | WQ_SYSFS, 0);
	ub_wq3 = alloc_workqueue("ub_wq3", WQ_UNBOUND | WQ_HIGHPRI | WQ_SYSFS, 0);
	ub_wq4 = alloc_workqueue("ub_wq4", WQ_UNBOUND | WQ_HIGHPRI | WQ_SYSFS, 0);
	if (!ub_wq1 || !ub_wq2 || !ub_wq3 || !ub_wq4)
		return -2;

	queue_work_on(0, ub_wq1, &work1);
//	msleep(10);
	queue_work_on(0, ub_wq1, &work2);
//	msleep(10);
	queue_work_on(0, ub_wq1, &work3);
//	msleep(10);
	queue_work_on(0, ub_wq1, &work4);

        return 0;
}

static void __exit wq_exit(void)
{
	/* destory work queue */
	destroy_workqueue(test_wq);
	destroy_workqueue(ub_wq1);
	destroy_workqueue(ub_wq2);
	destroy_workqueue(ub_wq3);
	destroy_workqueue(ub_wq4);
}

module_init(wq_init);
module_exit(wq_exit);
MODULE_AUTHOR("Sherlock");
MODULE_DESCRIPTION("The driver is for testing wq");
/*
ps -elf | grep kworker\/u
  7 root      [kworker/u16:0]
  24 root     [kworker/u18:0-e]
  35 root     [kworker/u19:0-e]
  46 root     [kworker/u20:0-e]
  54 root     [kworker/u17:1-e]
  63 root     [kworker/u17:2-e]
 104 root     [kworker/u18:2-e]
 280 root     [kworker/u20:3-e]
 403 root     [kworker/u19:3-e]
 813 root     [kworker/u21:0]
 814 root     [kworker/u22:0]
 815 root     [kworker/u23:0]
 816 root     [kworker/u24:0-t]
 817 root     [kworker/u25:0]
1700 root     [kworker/u24:1]
1701 root     [kworker/u19:1-e]
1703 root     grep kworker/u

 u means unbound work pool, n<num> num是work pool id，work pool根据nice, cpumask,
 no numa的值创建，如果这三个值一样，就用已有的work pool。比如上面的u16，17, 18
 19，20的nice是0， 21 - 25的nice是-20. 后面的n<num>:<num_1> num_1是线程池里线程
 的编号。

 不清楚-e/-t是什么意思?
*/
