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
	//��֤��������
	MetaClass(const MetaClass& mc)
	{}
	MetaClass& operator=(const MetaClass& mc)
	{
		return *this;
	}

public:
	//�ؼ����ʵĺ���
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
	
	//����ĳ����
	static MetaClass* FindObject(const MyString& cls)
	{
		for(MyClassSet_t::iterator it = ClassSets.begin(); it != ClassSets.end(); it++)
		{
			//if(cls.compare((*it)->ClassName))
			if(cls == (*it)->ClassName)
			{
				//����Object
				return (*it);
			}
		}
		return NULL;
	}

	//����ĳ���ഴ��
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
	//����ĳ��Ķ���
	template<class T>
	static T* ConstructObject()
	{
		return (T*)(ConstructObject(T::StaticClass()));
	}
	//��������ĳ��Ķ����м̳й�ϵ�ģ����ٺ٣�������һ��!

};
MyClassSet_t		MetaClass::ClassSets;

//�ؼ��ĺ꣬�����д���׳��������ú�������
#define DECLARE_MY_CLASS(ClassName) \
private: \
	static MetaClass* PrivateMetaClass; \
public: \
	/*������磬����������*/ \
	static MetaClass* StaticClass() {return PrivateMetaClass;} \
	/*���캯��*/ \
	static void InternalConstruct(void* p) \
	{ \
		/*ͨ��placement new���ø���Ĺ��캯���������ڴ��p*/ \
		new(p) ClassName(); \
	}

#define IMPLEMENT_MY_CLASS(ClassName) \
MetaClass* ClassName::PrivateMetaClass = new MetaClass( \
							#ClassName,  \
							ClassName::InternalConstruct, \
							sizeof(ClassName)); 

#if 1
//���ú궨��
class ObjectBase
{
private:
	static MetaClass* PrivateMetaClass;
public:
	/*������磬����������*/
	static MetaClass* StaticClass() {return PrivateMetaClass;}
	/*���캯��*/
	static void InternalConstruct(void* p)
	{
		/*ͨ��placement new���ø���Ĺ��캯���������ڴ��p*/
		new(p) ObjectBase();
	}
protected:
	//ʹ��Object���ܱ�new
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
	//ʹ��Object���ܱ�new
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
	//"����"һ����
	MetaClass* cls = MetaClass::FindObject("ObjectBase");
	ObjectBase* obj = MetaClass::ConstructObject(cls);
	if(cls)
		std::cout<<cls->ClassName.c_str()<<endl;
	if(obj)
		obj->ShowMe();

	//������������
	obj = MetaClass::ConstructObject(CommandA::StaticClass());
	if(obj)
		obj->ShowMe();	

	//�������������ɣ�ע��CommandBû�м̳���ObjectBase��һ��Ҳִ����ShowMe����
	//����Ϊ���պ���ƫ����ִ�У������Ǻ�������
	obj = MetaClass::ConstructObject("CommandB");
	if(obj)
		obj->ShowMe();	

	CommandB* cb = MetaClass::ConstructObject<CommandB>();
	if(cb)
		cb->ShowMe2();	


	system("pause");
	return 0;
}