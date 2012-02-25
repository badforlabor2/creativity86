#include <iostream>
#include <vector>
using namespace std;

class MetaClass;
class ObjectBase;

typedef std::string			MyString;
typedef std::vector<MetaClass*>		MyClassSet_t;

bool operator==(const MyString& str1, const MyString& str2)
{
	return (str1.compare(str2) == 0);
}


class MetaClass
{
public:
	MetaClass()
		: ClassName("none"),
			ClassConstructor(NULL),
			PropertySize(0)
	{
	
	}
	MetaClass(const MyString& nm, void(*cc)(void*), size_t size)
		: ClassName(nm), ClassConstructor(cc), PropertySize(size)
	{
		ClassSets.push_back(this);
	}

	MyString ClassName;
	void(*ClassConstructor)(void*);
	size_t PropertySize;

protected:
	//保证不允许复制
	MetaClass(const MetaClass& mc)
	{}
	MetaClass& operator=(const MetaClass& mc)
	{
		return *this;
	}

public:
	//关键性质的函数
	static MyClassSet_t		ClassSets;
	static void AddClass(MetaClass* cls)
	{
		ClassSets.push_back(cls);
	}
	static void RemoveClass(MetaClass* cls)
	{
		for(MyClassSet_t::iterator it = ClassSets.begin(); it != ClassSets.end(); it++)
		{
			if((*it) == cls)
			{
				ClassSets.erase(it);
				break;
			}
		}
	}
	
	//查找某个类
	static MetaClass* FindObject(const MyString& cls)
	{
		for(MyClassSet_t::iterator it = ClassSets.begin(); it != ClassSets.end(); it++)
		{
			//if(cls.compare((*it)->ClassName))
			if(cls == (*it)->ClassName)
			{
				//构造Object
				return (*it);
			}
		}
		return NULL;
	}

	//根据某个类创建
	static ObjectBase* ConstructObject(MetaClass* cls)
	{
		if(cls != NULL)
		{
			ObjectBase *p = (ObjectBase*)malloc(cls->PropertySize);
			(*cls->ClassConstructor)(p);
			return p;
		}
		return NULL;
	}
	static ObjectBase* ConstructObject(const MyString& cls)
	{
		return ConstructObject(FindObject(cls));
	}
	//创建某类的对象
	template<class T>
	static T* ConstructObject()
	{
		return (T*)(ConstructObject(T::StaticClass()));
	}
	//创建属于某类的对象（有继承关系的），嘿嘿，可以想一想!

};
MyClassSet_t		MetaClass::ClassSets;

//关键的宏，如果手写容易出错，所以用宏来代替
#define DECLARE_MY_CLASS(ClassName) \
private: \
	static MetaClass* PrivateMetaClass; \
public: \
	/*告诉外界，这个类的类型*/ \
	static MetaClass* StaticClass() {return PrivateMetaClass;} \
	/*构造函数*/ \
	static void InternalConstruct(void* p) \
	{ \
		/*通过placement new调用该类的构造函数，构造内存块p*/ \
		new(p) ClassName(); \
	}

#define IMPLEMENT_MY_CLASS(ClassName) \
MetaClass* ClassName::PrivateMetaClass = new MetaClass( \
							#ClassName,  \
							ClassName::InternalConstruct, \
							sizeof(ClassName)); 

#if 1
//不用宏定义
class ObjectBase
{
private:
	static MetaClass* PrivateMetaClass;
public:
	/*告诉外界，这个类的类型*/
	static MetaClass* StaticClass() {return PrivateMetaClass;}
	/*构造函数*/
	static void InternalConstruct(void* p)
	{
		/*通过placement new调用该类的构造函数，构造内存块p*/
		new(p) ObjectBase();
	}
protected:
	//使得Object不能被new
	ObjectBase() : test1(1), test2(2), test3("3456") {}
	int test1;
	int test2;
	std::string test3;
public:
	virtual void ShowMe()
	{
		std::cout<<test1<<"  "<<test2<<"--"<<test3.c_str()<<"<|>"<<endl;
	}
};
MetaClass* ObjectBase::PrivateMetaClass = new MetaClass(
							"ObjectBase",
							ObjectBase::InternalConstruct,
							sizeof(ObjectBase)); 

#else

class ObjectBase
{
	DECLARE_MY_CLASS(ObjectBase)
protected:
	//使得Object不能被new
	ObjectBase() : test1(1), test2(2), test3("3456") {}
	int test1;
	int test2;
	std::string test3;
public:
	virtual void ShowMe()
	{
		std::cout<<test1<<"  "<<test2<<"--"<<test3.c_str()<<"<|>"<<endl;
	}
};
IMPLEMENT_MY_CLASS(ObjectBase);
#endif
class CommandA: public ObjectBase
{
	DECLARE_MY_CLASS(CommandA)
public:
	virtual void ShowMe()
	{
		std::cout<<"command a:";
		ObjectBase::ShowMe();
	}
};
IMPLEMENT_MY_CLASS(CommandA);

class CommandB
{
	DECLARE_MY_CLASS(CommandB)
public:
	virtual void ShowMe1()
	{
		std::cout<<"command b!"<<endl;
	}
	virtual void ShowMe2()
	{
		std::cout<<"command b2!"<<endl;
	}
};
IMPLEMENT_MY_CLASS(CommandB);

int main()
{
	//"生成"一个类
	MetaClass* cls = MetaClass::FindObject("ObjectBase");
	ObjectBase* obj = MetaClass::ConstructObject(cls);
	if(cls)
		std::cout<<cls->ClassName.c_str()<<endl;
	if(obj)
		obj->ShowMe();

	//根据类来生成
	obj = MetaClass::ConstructObject(CommandA::StaticClass());
	if(obj)
		obj->ShowMe();	

	//根据名字来生成，注意CommandB没有继承自ObjectBase，一样也执行了ShowMe函数
	//是因为按照函数偏移来执行，而不是函数名字
	obj = MetaClass::ConstructObject("CommandB");
	if(obj)
		obj->ShowMe();	

	CommandB* cb = MetaClass::ConstructObject<CommandB>();
	if(cb)
		cb->ShowMe2();	


	system("pause");
	return 0;
}