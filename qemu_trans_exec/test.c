int add(int a, int b)
{
        int c;        

        c = a + b;

        return c;
}

int d;

int main()
{
	register unsigned long e = 1, f = 3, g = 4;

	e = d + 1;
	f = e + 2;
	g = f + 3;

	d = add(f, g);

        return 0;        
}
