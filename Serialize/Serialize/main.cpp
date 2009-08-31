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
	for(std::vector<UClass *>::size_type i=0; i<g_NativeClass.size(); i++){
		std::cout<<g_NativeClass[i]->name.data()<<std::endl;	
	}

	return 0;
}