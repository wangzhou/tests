#include <iostream>
#include <memory>

using namespace std;

int main()
{
	shared_ptr<int> p;

	p = make_shared<int>(100);

	cout << *p << "\n";

	return 0;
}
