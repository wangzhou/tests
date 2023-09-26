#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> v;
	
	cout << v.size() << "\n";
	if (v.empty())
		cout << "v is empty\n";

	cout << "cap is: " << v.capacity() << endl;
	
	v.emplace_back(1);
	v.emplace_back(2);
	v[2] = 3;
	v[3] = 4;

	cout << v.size() << "\n";
	cout << "cap is: " << v.capacity() << endl;

	return 0;
}
