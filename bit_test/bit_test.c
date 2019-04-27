#include <stdio.h>

int main()
{
        /* test normal << */
        int i = 1;
        int j = i << 5;
        long k;
        printf("test normal <<\n");
        printf("j = i << 5 --> i: %x, j: %x\n", i, j);
        printf("\n");

        /* test normal <<, shift > type size */
        i = 1;
        /* this is not right in gcc, j will be 1, not 0x100000000 */
        j = i << 32;
        printf("test normal <<, shift > type size\n");
        printf("j = i << 32 --> i: %lx, j: %lx\n", i, j);
        printf("\n");

         /* test normal <<, shift > type size */
        i = 1;
        /* this is right in gcc */
        k = (long)i << 32;
        printf("test normal <<, shift > type size\n");
        printf("size of long %d\n", sizeof(long));
        printf("k = i << 32 --> i: %lx, k: %lx\n", i, k);
        printf("\n");       

        /* test normal >> */
        i = 0xffe00000;
        /* as i is a int, it will remove to right with bit 1 in top bit */
        j = i >> 1;
        printf("test normal <<\n");
        printf("j = i >> 1 --> i: %x, j: %x\n", i, j);
        printf("\n");

         /* test |, type of two operands is different */
        int l = 0x10000000;
        long m = 0;
        long g = l | m;
        printf("test |, type of two operands is different\n");
        printf("g = l | m --> l: %lx, m: %llx, g: %llx\n", l, m, g);
        printf("\n");

	printf("0x%08x\n", 2);

        return 0;
}
