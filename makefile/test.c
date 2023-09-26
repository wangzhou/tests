#ifdef LIB
extern int add(int a, int b);
#else
static int add(int a, int b)
{
	return a - b;
}
#endif
int main()
{
	return add(1, 2);
}
