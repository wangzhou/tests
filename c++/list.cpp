#include <iostream>
#include <list>

using namespace std;

int main()
{
	list<int> q;
	
	int size = 10;
	cout << q.size() << "\n";
	if (q.empty())
		cout << "q is empty\n";
	
	q.push_back(1);
	q.push_back(2);
	q.push_back(3);
	q.push_back(4);
	q.push_back(5);
	q.push_back(6);
	q.push_front(100);
	cout << q.size() << "\n";
	for (auto it = q.begin(); it != q.end(); ++it) {
		cout << *it << endl;
	}
	cout << "list front: " << q.front() << "\n";
	q.pop_back();
	for (auto it = q.begin(); it != q.end(); ++it) {
		cout << *it << endl;
	}
	cout << "list front: " << q.front() << "\n";

	list<int>& t = q;
	cout << "t front: " << t.front() << "\n";

//	for (std::list<int>::reverse_iterator it = q.rbegin(); it != q.rend(); ++it) {
//		cout << *it << endl;
//	}

	for (std::list<int>::iterator it = q.begin(); it != q.end(); ++it) {
		cout << *it << endl;
	}

	list<int>::reverse_iterator it = q.rbegin();
	cout << "rbegin: " << *it << endl;
	//list<int>::iterator base = it.base();
	q.insert(--it.base(), 10);
	//q.insert(it_n, 11);

	cout << endl;
	for (std::list<int>::iterator it = q.begin(); it != q.end(); ++it) {
		cout << *it << endl;
	}

	return 0;
}
