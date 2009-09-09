/************************************************************************/
/* ����ģʽ������ģʽ�е�һ������ģʽ�����캯����˽�еģ����в�����new  */
/*	ʹ��ʱ��ֱ��Singleton::getInstance()                                */
/*	����Singleton *single = Singleton::getInstance();					*/
/************************************************************************/
class Singleton
{
private:
	static Singleton *p;
	Singleton();
	virtual ~Singleton();	//��������ɶʱ����ã�����취�����һ���ڲ���
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
	static Garbo garbo;		//���������ϵͳ����ʱ������øñ�������������
};
Singleton *Singleton::p = NULL;	//Ĭ�Ͼ���NULL���е���һ��
Singleton *Singleton::getInstance()
{
	if(p == NULL)
		p = new Singleton();
	return p;
}
Singleton::Singleton()
{

}
///�������������֪�Է񣬶��ҵ���ģʲôʱ��������������š�
~Singleton::Singleton()
{
	if(p != NULL)
		delete p;
	p = NULL;
}

/////////////////////////////////////////////////////////////////////////////
////-----��չ������ģʽʵ�������֣�����ʽ�Ͷ���ʽ�����������ʽ,�����Ƕ���ʽ
////-----����ʽ���̲߳���ȫ�ģ�����ʽ���ֳɰ�ȫ�ġ�����Ҫ�޸�����ʽ
//1����ͬ�㣺���ַ�ʽ�Ĺ��캯������˽�еģ�����Ľӿڶ��ǹ�������
//2����ͬ�㣺����ʽ������װ�ص�ʱ��ֱ�ӵõ������ʵ��������˵��ǰ�ڰ󶨵ģ�
//����ʽ�Ǻ��ڰ󶨵ģ�����ص�ʱ��uniSingleton�ǿյģ�����Ҫ��ʱ��ű�����
//�ҽ�����һ�Ρ�����ʽ���ٶȿ죬Ч�ʸߣ����Ǻķ�ϵͳ��Դ������ʽ���෴����
//��ʽ������һ�����⣬���Ǻ��ڰ󶨲���ȷ������ֻ�ܱ�ʵ����һ�Σ�����Ҫ��ָ
//ʾ���Ƿ�ʵ�����ı�־����1����������������1���̷߳��ʡ������Ϳ���ȷ������
//ֻ��ʵ����һ�Ρ�

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
	return p;	//�Ѿ�����ʼ��
}

*/