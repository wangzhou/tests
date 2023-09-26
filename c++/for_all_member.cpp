#include <iostream>
#include <list>

using namespace std;

int main()
{
	list<int> q;
	
	q.push_back(1);
	q.push_back(2);
	q.push_back(3);
	q.push_back(4);
	q.push_back(5);
	q.push_back(6);

	for (auto it : q) {
		cout << it << endl;
	}

	return 0;
}
