#include "THHashTable.h"
#include <string>
#include <iostream>
using namespace std;

template<class T>
void THLink<T>::insert(T data)
{
	THLinkNode<T> *temp = new THLinkNode<T>();
	temp->nValue = data;
	temp->next = 0;
	if(first == 0){
		first = temp;
		current = temp;
	}else{
		current->next = temp;
		current = temp;
	}
}
template<class T>
bool THLink<T>::insertUnique(T data)
{
	THLinkNode<T> *temp = new THLinkNode<T>();
	THLinkNode<T> *curr;
	temp->nValue = data;
	temp->next = 0;
	curr = first;
	while(curr){
		if(data == curr->nValue)
			return false;
		curr = curr->next;
	}
	if(curr == 0){
		first = temp;
		current = temp;
	}else{
		current->next = temp;
		current = temp;
	}
	return true;

}
template<class T>
void THLink<T>::show()
{
	THLinkNode<T> *curr;
	curr = first;
	while(curr != 0){
		cout<<curr->nValue<<"  ";
		curr = curr->next;
	}
}
template<class T>
T *THLink<T>::find(T *t) const
{
	THLinkNode<T> *curr;
	curr = first;
	while(curr){
		// T 需要重写"=="运算符
		if(*t == curr->nValue){
			return &(curr->nValue);
		}
		curr = curr->next;
	}
	return 0;
}
template<class T>
bool THLink<T>::deleteFromEnd()
{
	THLinkNode<T> *curr;
	curr = first;
	if(curr->next == 0){
		delete curr;
		curr = 0;
		first = 0;
		current = 0;
		return true;
	}
	while(curr){
		if(curr->next == current){
			curr->next = 0;
			delete current;
			current = curr;
			return true;
		}
		curr = curr->next;
	}
	return false;
}
template<class T>
void THLink<T>::deleteAll()
{
	THLinkNode<T> *curr;
	while(first != 0){
		curr = first;
		first = curr->next;
		delete curr;
	}
}
bool THHashElement::operator == (const THHashElement e)
{
	if(strcmp(key, e.key) == 0){
		return true;
	}else 
		return false;
}
/************************************************************************/
/* THHashTable                                                                     */
/************************************************************************/
//key-keyValue; 这个key为字符串，keyValue为int型值
bool THHashTable::addOne(const char *key, const char *keyValue)
{
	THHashElement e;
	int index;
	strncpy(e.key, key, sizeof(key));
	strncpy(e.keyValue, keyValue, sizeof(key));
	index = hash(key) % size;
	return table[index].insertUnique(e);
}
char *THHashTable::getOne(const char *key) const
{
	THHashElement e, *ret;
	int index;
	strncpy(e.key, key, strlen(key)+1);
	index = hash(key) % size;
	ret=table[index].find(&e);
	if(ret != 0){
		return ret->keyValue;
	}else
		return "";
}
int THHashTable::hash(const char *key) const
{
	unsigned int nHash = 0;
	while(*key){
		nHash = (nHash << 5) + nHash + *key++;
	}
	return nHash%size;
}
bool THHashTable::serialize()
{
//序列化是一个问题！如何才能更好的满足序列化的要求呢？扩展性必须高，这是肯定的了！	
	return false;
}

//void main()
//{
//	THLink<int> link;
//	char str[10];
//	link.insert(10);
//	link.insert(11);
//	link.show();
//
//	THHashTable ht;
//	ht.addOne("liu", "liubo1");
//	ht.addOne("liu2", "liubo2");
//	ht.addOne("liu3", "liubo3");
//	ht.addOne("liu4", "liubo4");
//	strcpy(str, ht.getOne("liu3"));
//	cout<<str;
//
//	system("pause");
//}