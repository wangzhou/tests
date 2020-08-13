#include <stdio.h>

union test {
        int a;
        char b;
};

main()
{
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
