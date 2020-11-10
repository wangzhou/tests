#include <stdio.h>
#include <sys/queue.h>
#include <stdlib.h>

struct entry {
	int data;
	TAILQ_ENTRY(entry) entries;
};
TAILQ_HEAD(list_head, entry);	

int main()
{
	struct list_head head;
	TAILQ_INIT(&head);
	struct entry *e1, *e2, *e3, *tmp;

	e1 = malloc(sizeof(struct entry));
	e1->data = 2;
	TAILQ_INSERT_TAIL(&head, e1, entries);

	e2 = malloc(sizeof(struct entry));
	e2->data = 3;
	TAILQ_INSERT_TAIL(&head, e2, entries);

	e3 = malloc(sizeof(struct entry));
	e3->data = 4;
	TAILQ_INSERT_TAIL(&head, e3, entries);

	TAILQ_FOREACH(tmp, &head, entries) {
		printf("---> %d\n", tmp->data);
	}

	return 0;	
}

1. free list
