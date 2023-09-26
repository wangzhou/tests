#include <iostream>
#include <queue>

using namespace std;

int main()
{
	queue<int> q;
	
	int size = 10;
	cout << q.size() << "\n";
	if (q.empty())
		cout << "q is empty\n";
	
	q.push(1);
	cout << q.size() << "\n";

	q.pop();
	q.pop();
	cout << q.size() << "\n";

	return 0;
}
