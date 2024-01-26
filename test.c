
#define A 1
#define B 4
#define G (A | B)

#define _TEST(x) #x
#define TEST(x) _TEST(x)

int main()
{
	int a = 1, b = 2, ret = 0;


	__asm__ __volatile__ (
		"add %0, %1, %1\n"
		"add %0, %2, %2\n"
		".word " TEST(G)
		: "=r" (ret)
		: "r" (a), "r" (b)
		:
	);

	return ret;
}
