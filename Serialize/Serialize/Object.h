/************************************************************************/
/* 创建Object类，所有类的基类                                           */
/************************************************************************/
#include<iostream>
#include<string>
#include<vector>
#include"Archives.h"
using namespace std;

class UClass;
vector<UClass *> g_NativeClass;
class UClass
{
public:
	string name;
	int ObjectTypeSize;
	void (*CallObjectConstructor)(void *);
	UClass(string inName, int nSize, void (*inConstructor)(void *))
	{
		name = inName;
		ObjectTypeSize = nSize;	
		g_NativeClass.push_back(this);
	}
};

class UObject
{
public:
	UClass *Class;
	virtual void serialize(Archives &arc){};
	static UClass PrivateStaticClass;
	void *operator new (size_t size, void *p)
	{
		return p;
	}
	static void ConstructObject(void *p)
	{
		new (p) UObject();
		((UObject *)p)->Class = &PrivateStaticClass;
	}
};
UClass UObject::PrivateStaticClass("UObject", sizeof(UObject), UObject::ConstructObject);

UClass *FindNativeClass(string ClassName)
{
	for(std::vector<UClass *>::size_type i=0; i<g_NativeClass.size(); i++){
		if(ClassName == g_NativeClass[i]->name);
			return g_NativeClass[i];
	}
	return NULL;
}
UObject *CreateObject(string ClassName)
{
	UClass *pClass = FindNativeClass(ClassName);
	UObject *obj = (UObject *)malloc(pClass->ObjectTypeSize);
	pClass->CallObjectConstructor(obj);
	return obj;
}