#include <iostream>
#include <thread>
#define endl '\n'

using namespace std;

void threadFunction(){
	cout << "Hello from thread!!" << endl;
}

int main()
{
	thread th(threadFunction);
	cout << "Hello from main!" << endl;
	th.join();
	return 0;
}
