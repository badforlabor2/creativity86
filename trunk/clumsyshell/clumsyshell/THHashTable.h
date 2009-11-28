/************************************************************************/
/* 创建一个hashtable                                                    */
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
	bool find(T *t) const;
	bool deleteFromEnd();
	void deleteAll();
	bool isEmpty(){return (first==0);}
	~THLink()
	{
		deleteAll();
	}
};

/************************************************************************/
/*	hashtable的特征                                                     */
/*	存在一个数值对<key, keyValue>，通过key能立刻得到KeyValue			*/
/*	一般来说，key和value的值为int或者string类型，常见的都是string，所以 */
/*	参照string来做*/
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
	void addOne(const char *key, const char *keyValue);
	char *getOne(const char *key) const;
	int hash(const char *key) const;
};







#endif