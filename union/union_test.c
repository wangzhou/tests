#include <stdio.h>

union test {
        int a;
        char b;
}

main()
{
        union test u1;
        
        u1.a = 0x12345678;
        printf("u1.b: %x\n", u1.b);
}
