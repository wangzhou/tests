#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

volatile atomic_int a;
volatile atomic_int tmp;

int main(void)
{

        tmp = __atomic_load_n(&a, __ATOMIC_SEQ_CST);
//	__atomic_fetch_add(&a, 1, __ATOMIC_ACQUIRE);
	__atomic_fetch_add(&a, 1, __ATOMIC_SEQ_CST);

	return tmp;
}
