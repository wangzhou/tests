#include <iostream>
#include <queue>
#include <ucontext.h>

using namespace std;

void exit_test(void)
{
	exit(0);
}

int main()
{
	int input_number, i = 0, j = 0;
	ucontext_t send, curr;
	queue<int> q;
	
	cout << "Please input package number: " << endl;
	cin >> input_number; 

	getcontext(&send);

	if (i < input_number)
		q.push(i++);

	if (i == 3) {
		swapcontext(&curr, &send);
	}

	if (i == 4) {
		makecontext(&send, exit_test, 0);
	}

	if (j++ < input_number) {
		int e = q.front();
		cout << "Out put: " << dec << e << endl;
		q.pop();
	}
	
	if (j < input_number)
		setcontext(&send);

	return 0;
}
