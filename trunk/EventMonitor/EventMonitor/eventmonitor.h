/************************************************************************/
/* 用模板实现事件监视器                                                 */
/************************************************************************/
#include <iostream>
#include <map>
#include <vector>

///事件参数
class THEventArgs
{
	int id;
};

///函数基类
class THFuncBase
{
public:
	virtual ~THFuncBase() {}
	virtual bool operator()(const THEventArgs& args) = 0;
};

///函数指针类
//class THFunc : public THFuncBase
//{
//public:
//	typedef bool (Func)(const THEventArgs& args);
//	THFunc(Func* f)
//		:	func(f)
//	{
//	
//	}
//	bool operator()(const THEventArgs& args)
//	{
//		return func(args);
//	}
//
//	Func *func;
//};

template<typename T>
class THMemberFunc : public THFuncBase
{
public:
	//成员函数指针
	typedef bool (T::*Func)(const THEventArgs& args);
	THMemberFunc(Func f, T* obj)
		:	m_func(f), m_obj(obj)
	{

	}
	virtual bool operator()(const THEventArgs& args)
	{
		return (m_obj->*m_func)(args);
	}

	Func m_func;
	T *m_obj;
};

///捆绑函数
class THSubscriber
{
public:
	template<typename T>
	THSubscriber(bool (T::*fun)(const THEventArgs&), T* obj)
		:	m_functor_impl(new THMemberFunc<T>(fun, obj))
	{
		std::cout<<"construct subscriber!"<<std::endl;
	}
	~THSubscriber()
	{

	}
	void operator()(const THEventArgs& args)
	{
		(*m_functor_impl)(args);
	}
	void destroy()
	{
		delete m_functor_impl;
		m_functor_impl = 0;
	}
private:
	THFuncBase* m_functor_impl;
};

///一个事件可能对应多个处理函数，所以，再次对处理函数进行封装
class THEventMonitor
{
public:
	typedef std::vector<THSubscriber> FuncsContainer;
	THEventMonitor(int id)
		:	m_id(id)
	{

	}
	~THEventMonitor()
	{
		for(FuncsContainer::iterator it = m_funs.begin(); it != m_funs.end(); it++)
		{
			(*it).destroy();
		}
		m_funs.clear();
	}
	FuncsContainer m_funs;
	void subscribe(const THSubscriber &slot)
	{
		m_funs.push_back(slot);
	}
	void operator()(const THEventArgs& args)
	{
		for(FuncsContainer::iterator it = m_funs.begin(); it != m_funs.end(); it++)
		{
			(*it)(args);
		}
	}
	int m_id;
};

///事件的集合，管理所有的事件
class THEventSet
{
public:
	typedef std::map<int, THEventMonitor*> EventMap;
	EventMap m_events;
	THEventMonitor* getEventObject(int id, bool autoAdd = false)
	{
		EventMap::iterator it = m_events.find(id);
		if(it == m_events.end())
		{
			if(autoAdd)
			{
				m_events[id] = new THEventMonitor(id);
				return m_events.find(id)->second;
			}
			else
			{
				return 0;
			}
		}
		return it->second;
	}
	void subscribeEvent(int id, THSubscriber subs)
	{
		getEventObject(id, true)->subscribe(subs);				
	}
	void fireEvent(int id, THEventArgs& args)
	{
		THEventMonitor *ev = getEventObject(id);
		if(ev)
		{
			(*ev)(args);
		}
	}
	void removeEvent(int id)
	{
		EventMap::iterator it = m_events.find(id);
		if(it != m_events.end())
		{
			delete it->second;
			m_events.erase(it);
		}
	}
};

///一个例子
class AExample
{
public:
	bool SayHello(const THEventArgs& args)
	{
		std::cout<<"hello, we've done!"<<std::endl;		
		return true;
	}
	void show()
	{
		THEventSet es;
		es.subscribeEvent(0, THSubscriber(&AExample::SayHello, this));
		THEventArgs eargs;
		es.fireEvent(0, eargs);
	}

};