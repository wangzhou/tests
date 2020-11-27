#include <stdio.h>

union test {
        int a;
        char b;
};

struct sgl {
	int data;
	struct sgl *next;
};

struct req {
	union {
		void *src;
		struct sgl *sgl;
	};

	int data_fmt;
};

main()
{
	struct sgl sgl;

	struct req req_src, req_sgl;
	struct req *tmp;
	
	req_src.src = (void *)66;
	req_src.data_fmt = 0;

	req_sgl.sgl = &sgl;
	req_sgl.data_fmt = 1;
	sgl.data = 55;
	sgl.next = NULL;

	tmp = &req_src;
	if (!tmp->data_fmt)
		printf("--> %lu\n", tmp->src);
	else
		printf("--> %lu\n", tmp->sgl->data);

	tmp = &req_sgl;
	if (!tmp->data_fmt)
		printf("--> %lu\n", tmp->src);
	else
		printf("--> %lu\n", tmp->sgl->data);

	union {
		long k;
		char i[2];
	} *s, a;

        union test u1;
        
        u1.a = 0x12345678;
        printf("u1.b: %x\n", u1.b);

	s = &a;
	s->i[0] = 0x39;
	s->i[1] = 0x38;

	printf("%x\n", a.k);
}
