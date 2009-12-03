/************************************************************************/
/* 创建一个hashtable                                                    */
/************************************************************************/
#ifndef THHASHTABLE_H
#define THHASHTABLE_H
#include "THArchive.h"

template<class T>
struct THLinkNode
{
	T nValue;
	THLinkNode *next;
};

template<class T>
class THLink
{
protected:
	THLinkNode<T> *first, *current;
	int size;
public:
	THLink()
	{
		first =0;
		current = 0;
		size = 0;
	}
	void show();
	void insert(T data);
	bool insertUnique(T data);
	T	*find(T *t) const;
	bool deleteFromEnd();
	void deleteAll();
	int getSize() const
	{
		return size;
	}
	void serialize(THArchive &arc);
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
	void serialize(THArchive &arc)
	{
		arc<<key<<keyValue;
	}
};
class THHashLink : public THLink<THHashElement>
{
public:
	void serialize(THArchive &arc)
	{
		THLinkNode<THHashElement> *curr;
		if(arc.type == OUTPUT){
			arc<<size;
			curr = first;
			while(curr){
				curr->nValue.serialize(arc);
				curr = curr->next;
			}		
		}else if(arc.type == INPUT){
			int temp = 0;
			arc<<temp;
			for(int i=0; i<temp; i++){
				THHashElement e;
				e.serialize(arc);
				insert(e);
			}
		}
	}
};

class THHashTable
{
private:
	static const int size = 163;
	THHashLink table[size];
public:
	bool addOne(const char *key, const char *keyValue);
	char *getOne(const char *key) const;
	int hash(const char *key) const;
	bool serialize(THArchive &arc);
};
#endif