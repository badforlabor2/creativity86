#include "THArchive.h"
#include <iostream>
using namespace std;

int main()
{
	THTestArchive ta;
	//ta.age = 10;
	//strncpy(ta.name, "liubo", 6);
	//THArchiveWriter aw("ta");
	//ta.serialized(aw);
	THArchiveReader ar("ta");
	ta.serialized(ar);
	cout<<ta.name<<ta.age;

	system("pause");
	return 0;
}