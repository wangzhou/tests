#define FUNC add

int add(int a, int b)
{
        int c;        

        c = a + b;

        return c;
}

int main()
{
	int d;

	d = FUNC(2, 5);

        return 0;        
}
