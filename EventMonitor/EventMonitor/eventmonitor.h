/************************************************************************/
/* ��ģ��ʵ���¼�������                                                 */
/************************************************************************/
#include <iostream>
#include <map>
#include <vector>

///�¼�����
class THEventArgs
{
	int id;
};

///��������
class THFuncBase
{
public:
	virtual ~THFuncBase() {}
	virtual bool operator()(const THEventArgs& args) = 0;
};

///����ָ����
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
	//��Ա����ָ��
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

///������
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

///һ���¼����ܶ�Ӧ��������������ԣ��ٴζԴ��������з�װ
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

///�¼��ļ��ϣ��������е��¼�
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

///һ������
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