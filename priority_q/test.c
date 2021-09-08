#include <stdio.h>

#define get_left(i)		(2 * (i) + 1)
#define get_right(i)		(2 * (i) + 2)
#define get_father(i)		((i) % 2 ? (i) / 2 : (i) / 2 - 1)

/*
 * data1 value <  data2 value, return < 0
 * data1 value == data2 value, return = 0
 * data1 value >  data2 value, return > 0
 */
typedef int (*comp_fun)(void *data1, void *data2);

struct pri_q {
	void **q;
	int length;
	int tail;
	comp_fun comp;
};

struct pri_q *pri_q_create(int length, comp_fun comp)
{
	struct pri_q *ret;

	ret = calloc(1, sizeof(*ret));
	if (!ret)
		return NULL;

	ret->q = calloc(1, sizeof(void *) * length);
	if (!ret->q) {
		free(ret);
		return NULL;
	}
	ret->length = length;
	ret->comp = comp;
	ret->tail = 0;

	return ret;
}

void swap(void *data1, void *data2)
{
	void *tmp;

	tmp = data1;
	data1 = data2;
	data2 = tmp;
}

/* do as bigger value priority */
void pri_q_insert(struct pri_q *pri_q, void *data)
{
	assert(pri_q->tail < pri_q->length - 1);
	int i = pri_q->tail;

	pri_q->q[pri_q->tail] = data;
	pri_q->tail++;

	while (i && pri_q->comp(data, pri_q->q[get_father(i)])) {
		swap(pri_q->q[i], pri_q->q[get_father(i)]);
		i = get_father(i);
	}
}

void *pri_q_get(struct pri_q *pri_q)
{
	void *ret = pri_q->q[0];
	int i = 0, j;
 
	swap(pri_q->q[0], pri_q->q[pri_q->tail]);

	/* fixme: should have child, need check */
	while (pri_q->comp(pri_q->q[i], pri_q->q[get_left(i)]) < 0 ||
	       pri_q->comp(pri_q->q[i], pri_q->q[get_right(i)]) < 0) {
		if (pri_q->q[get_left(i)] > pri_q->q[get_right(i)])
			j = get_left(i);
		else
			j = get_right(i);

		swap(pri_q->comp(pri_q->q[i], pri_q->q[i]);
	}

	return ret;
}

void pri_q_release(struct pri_q *pri_q)
{
	/* need user to free data ahaed */
	free(pri_q->q);
	free(pri_q);
}

int main()
{
	/* test case */

	return 0;
}
