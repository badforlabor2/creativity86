/************************************************************************/
/* 单例模式，设置模式中的一个经典模式，构造函数是私有的，所有不能用new  */
/*	使用时，直接Singleton::getInstance()                                */
/*	或者Singleton *single = Singleton::getInstance();					*/
/************************************************************************/
class Singleton
{
private:
	static Singleton *p;
	Singleton();
	virtual ~Singleton();	//析构函数啥时候掉用，解决办法，添加一个内部类
public:
	static Singleton *getInstance();
private:
	class Garbo
	{
	public:
		~Garbo(){
			if(Singleton::p != NULL){
				delete p;
			}
		}
	};	
	static Garbo garbo;		//这个变量在系统结束时，会调用该变量的析构函数
};
Singleton *Singleton::p = NULL;	//默认就是NULL，有点多此一举
Singleton *Singleton::getInstance()
{
	if(p == NULL)
		p = new Singleton();
	return p;
}
Singleton::Singleton()
{

}
///这个析构函数不知对否，而且单例模什么时候调用析构函数呐。
~Singleton::Singleton()
{
	if(p != NULL)
		delete p;
	p = NULL;
}

/////////////////////////////////////////////////////////////////////////////
////-----扩展，单例模式实现有两种，懒汉式和饿汉式，上面的懒汉式,下面是饿汉式
////-----懒汉式是线程不安全的，饿汉式是现成安全的。所以要修改懒汉式
//1、相同点：两种方式的构造函数都是私有的，对外的接口都是工厂方法
//2、不同点：饿汉式是在类装载的时候直接得到该类的实例，可以说是前期绑定的；
//懒汉式是后期绑定的，类加载的时候uniSingleton是空的，在需要的时候才被创建
//且仅创建一次。饿汉式的速度快，效率高，但是耗费系统资源；懒汉式则相反。懒
//汉式还存在一个问题，就是后期绑定不能确保对象只能被实例化一次，这需要对指
//示类是否实例化的标志设置1个互斥锁，仅允许1个线程访问。这样就可以确保对象
//只被实例化一次。

/*
class Singleton
{
private:
	Singleton();
	~Singleton();
	static Singleton *p;
public:
	static Singleton *getInstance();
};
Singleton *Singleton::p = new Singleton();

Singleton *Singleton::getInstance()
{
	return p;	//已经被初始化
}

*/