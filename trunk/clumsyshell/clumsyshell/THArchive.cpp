#include "THArchive.h"
#include <iostream>
using namespace std;

int main()
{
	THTestArchive ta;
	//ta.age = 10;
	//strncpy(ta.name, "liubo", 6);
	//THArchiveWriter aw("ta");
	//ta.serialized(aw, 0);
	THArchiveReader ar("ta");
	ta.serialized(ar, 1);
	cout<<ta.name<<ta.age;

	system("pause");
	return 0;
}