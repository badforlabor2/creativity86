#include"Object.h"
//#include"Archives.h"
#include<iostream>
using namespace std;

class Dog : public UObject
{
	int weight, color;
public:
	virtual void serialize(Archives &arc)
	{
		arc<<weight<<color;
	}
	static UClass PrivateStaticClass;
	void* operator new(size_t size, void *p)
	{
		return p;
	}
	static void ConstructObject(void *p)
	{
		new(p) Dog();
		((UObject *)p)->Class = &PrivateStaticClass;
	}
};
UClass Dog::PrivateStaticClass("Dog", sizeof(Dog), Dog::ConstructObject);

class Player : public UObject
{
	int course, score, friends;
public:
	virtual void serialize(Archives &arc)
	{
		arc<<course<<score<<friends;
	}
	static UClass PrivateStaticClass;
	void *operator new(size_t size, void *p)
	{
		return p;
	}
	static void ConstructObject(void *p)
	{
		new (p) Player();
		((UObject *)p)->Class = &PrivateStaticClass;
	}
};
UClass Player::PrivateStaticClass("Player", sizeof(Player), Player::ConstructObject);


int main()
{
	//for(std::vector<UClass *>::size_type i=0; i<g_NativeClass.size(); i++){
	//	std::cout<<g_NativeClass[i]->name.data()<<std::endl;	
	//}

#if 0
	int a = 1;
	float b = 2.0f;
	MyString str("1234");

	ArchivesWrite writer("c:\\test1.txt");
	writer<<a<<b<<str;
	writer.flush();


	int aa = 0;
	float bb = 0.f;
	MyString str2;
	ArchivesReader reader("c:\\test1.txt");
	reader<<aa<<bb<<str2;
	reader.flush();
	std::cout<<"\n------"<<aa<<" "<<bb<<" "<<str2.GetPtr()<<std::endl;

#else

	int a = 1;
	float b = 2.0f;
	MyString str("1234");

	MemoryWriter writer;
	writer<<a<<b<<str;
	writer.flush();


	int aa = 0;
	float bb = 0.f;
	MyString str2;
	MemoryReader reader(writer.GetMyMem());
	reader<<aa<<bb<<str2;
	reader.flush();
	std::cout<<"\n------"<<aa<<" "<<bb<<" "<<str2.GetPtr()<<std::endl;

#endif

	system("pause");
	return 0;
}