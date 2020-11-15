#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

struct your_list_node {
	int data;
	TAILQ_ENTRY(your_list_node) node;
};
TAILQ_HEAD(list_head, your_list_node);	

int main()
{
	struct list_head head;
	TAILQ_INIT(&head);

	struct your_list_node e1, e2, e3, e4, e5, *tmp;

	e1.data = 1;
	TAILQ_INSERT_TAIL(&head, &e1, node);

	e2.data = 2;
	TAILQ_INSERT_TAIL(&head, &e2, node);

	e3.data = 3;
	TAILQ_INSERT_TAIL(&head, &e3, node);

	e4.data = 4;
	TAILQ_INSERT_TAIL(&head, &e4, node);

	TAILQ_FOREACH(tmp, &head, node) {
		printf("---> %d\n", tmp->data);
	}
	printf("\n");

	e5.data = 5;
	TAILQ_INSERT_BEFORE(&e4, &e5, node);

	TAILQ_FOREACH(tmp, &head, node) {
		printf("---> %d\n", tmp->data);
	}
	printf("\n");

	tmp = TAILQ_LAST(&head, list_head);
	printf("last: %d\n\n", tmp->data);

	TAILQ_REMOVE(&head, &e2, node);
	TAILQ_FOREACH(tmp, &head, node) {
		printf("---> %d\n", tmp->data);
	}
	printf("\n");

	return 0;	
}
