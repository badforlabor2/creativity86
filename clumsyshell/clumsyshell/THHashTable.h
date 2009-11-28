/************************************************************************/
/* ����һ��hashtable                                                    */
/************************************************************************/
#ifndef THHASHTABLE_H
#define THHASHTABLE_H
template<class T>
struct THLinkNode
{
	T nValue;
	THLinkNode *next;
};

template<class T>
class THLink
{
private:
	THLinkNode<T> *first, *current;
public:
	THLink()
	{
		first =0;
		current = 0;
	}
	void show();
	void insert(T data);
	bool insertUnique(T data);
	T	*find(T *t) const;
	bool deleteFromEnd();
	void deleteAll();
	bool isEmpty(){return (first==0);}
	~THLink()
	{
		deleteAll();
	}
};

/************************************************************************/
/*	hashtable������                                                     */
/*	����һ����ֵ��<key, keyValue>��ͨ��key�����̵õ�KeyValue			*/
/*	һ����˵��key��value��ֵΪint����string���ͣ������Ķ���string������ */
/*	����string����*/
/************************************************************************/
class THHashElement
{
public:
	char key[10];
	char keyValue[30];
public:
	THHashElement()
	{
		key[0] = 0;
		keyValue[0] = 0;
	}
	bool operator == (THHashElement e);

};
class THHashTable
{
private:
	static const int size = 163;
	THLink<THHashElement> table[size];
public:
	bool addOne(const char *key, const char *keyValue);
	char *getOne(const char *key) const;
	int hash(const char *key) const;
};







#endif