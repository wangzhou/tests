#include <stdio.h>
#include <malloc.h>

#define get_left(i)		(2 * (i) + 1)
#define get_right(i)		(2 * (i) + 2)
#define get_father(i)		((i) % 2 ? (i) / 2 : (i) / 2 - 1)
#define is_valid(i, tail)	((i) < (tail))

#define E_FULL			(-1)
#define DEBUG

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

void swap(void **data1, void **data2)
{
	void *tmp;

	tmp = *data1;
	*data1 = *data2;
	*data2 = tmp;
}

/* do as bigger value priority */
int pri_q_insert(struct pri_q *pri_q, void *data)
{
#ifdef DEBUG
	printf("--> debug: insert %ld\n", (long)data);
#endif
	int i = pri_q->tail;
	void **q = pri_q->q;

	if (i >= pri_q->length - 1)
		return E_FULL;

	q[i] = data;
	pri_q->tail++;

	while (i && pri_q->comp(data, q[get_father(i)])) {
		swap(&q[i], &q[get_father(i)]);
		i = get_father(i);
	}

	return 0;
}

void *pri_q_get(struct pri_q *pri_q)
{
	void **q = pri_q->q;
	int i = 0, j;
	int tail = pri_q->tail;
	void *ret;

	if (!tail)
		return NULL;

	ret = q[0];
#ifdef DEBUG
	printf("--> debug: get %ld\n", (long)ret);
#endif
	swap(&q[0], &q[tail - 1]);
	pri_q->tail--;
	tail--;

	while (is_valid(get_left(i), tail) && pri_q->comp(q[i], q[get_left(i)]) < 0 ||
	       is_valid(get_right(i), tail) && pri_q->comp(q[i], q[get_right(i)]) < 0) {

		if (is_valid(get_left(i), tail) && is_valid(get_right(i), tail)) {
			if (q[get_left(i)] > q[get_right(i)])
				j = get_left(i);
			else
				j = get_right(i);
		} else if (is_valid(get_left(i), tail)) {
			j = get_left(i);
		} else {
			j = get_right(i);
		}

		swap(&q[i], &q[j]);
		i = j;
	}

	return ret;
}

void pri_q_release(struct pri_q *pri_q)
{
	/* need user to free data ahaed */
	free(pri_q->q);
	free(pri_q);
}

void pri_q_print_long(struct pri_q *pri_q)
{
	void **q = pri_q->q;
	int i;

	for (i = 0; i < pri_q->tail; i++)
		printf("%ld ", (long)q[i]);

	printf("\n");
}

int comp_long(void *data1, void *data2)
{
	return (long)data1 - (long)data2;
}

int main()
{
	struct pri_q *h_q;

	h_q = pri_q_create(20, comp_long);

	pri_q_insert(h_q, (void *)2);
	pri_q_insert(h_q, (void *)4);
	pri_q_insert(h_q, (void *)6);
	pri_q_insert(h_q, (void *)8);
	pri_q_print_long(h_q);

	pri_q_get(h_q);
	pri_q_print_long(h_q);

	pri_q_insert(h_q, (void *)7);
	pri_q_print_long(h_q);

	pri_q_release(h_q);


	return 0;
}
