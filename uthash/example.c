#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./uthash.h"

struct my_struct {
    int id;                    /* 这个id后面我们用来做my_struct的索引 */
    char name[10];
    UT_hash_handle hh;         /* 要hash的数据结构里必须放一个这个句柄，必须写成hh */
};

/* 定义hash表的表头，需要初始化成NULL */
struct my_struct *users = NULL;

/* 所有的uthash操作都是HASH_xx的定义，我们这里封装一层函数 */
void add_user(int user_id, char *name)
{
    struct my_struct *s;

    HASH_FIND_INT(users, &user_id, s);  /* 查看user_id为key的数据是否存在，返回数据的指针，s为NULL，数据不存在 */
    if (s == NULL) {
        s = (struct my_struct*)malloc(sizeof(struct my_struct));
        s->id = user_id;
        HASH_ADD_INT(users, id, s);  /* user_id为key插入s到users */
    }
    strcpy(s->name, name);
}

struct my_struct *find_user(int user_id)
{
    struct my_struct *s;

    HASH_FIND_INT(users, &user_id, s);
    return s;
}

void delete_user(struct my_struct *user)
{
    HASH_DEL(users, user);  /* 直接指向数据的指针, 用这个作为删除的标记 */
    free(user);
}

void delete_all()
{
    struct my_struct *current_user, *tmp;

    HASH_ITER(hh, users, current_user, tmp) {
        HASH_DEL(users, current_user);
        free(current_user);  /* 删除操作并不影响数据内存，需要用户显示释放数据内存 */
    }
}

void print_users()
{
    struct my_struct *s;

    for(s=users; s != NULL; s=(struct my_struct*)(s->hh.next)) {
        printf("user id %d: name %s\n", s->id, s->name);
    }
}

int name_sort(struct my_struct *a, struct my_struct *b)
{
    return strcmp(a->name, b->name);
}

int id_sort(struct my_struct *a, struct my_struct *b)
{
    return (a->id - b->id);
}

void sort_by_name()
{
    HASH_SORT(users, name_sort);
}

void sort_by_id()
{
    HASH_SORT(users, id_sort);
}

int main()
{
	struct my_struct *tmp;
	int num;

	add_user(5, "wang");
	add_user(1, "zheng");
	add_user(4, "xu");
	add_user(3, "fang");
	add_user(2, "huang");

	print_users();

	printf("\n");
	sort_by_id();
	print_users();

	printf("\n");
	sort_by_name();
	print_users();

	printf("\n");
	tmp = find_user(4);
	delete_user(tmp);
	print_users();

	printf("\n");
	num = HASH_COUNT(users);
	printf("there is %d elements\n", num);

	printf("\n");
	HASH_CLEAR(hh, users);
	assert(!users);

	return 0;
}
