#include<iostream>
#include"MyHashTable.h"
using namespace std;

int main()
{
	MyHashTable my("save");
	cout<<my.getCmd("cmd")<<endl;
	cout<<my.getCmd("show")<<endl;
	my.addCmd("cmd", "cmd.exe");
	my.addCmd("show", "c;\\¾«²Ê\\¾«²Ê.exe");
	my.saveTable();

	return 0;
}