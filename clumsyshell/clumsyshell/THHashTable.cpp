#include "THHashTable.h"
#include "THGuard.h"
#include <string>
#include <iostream>
using namespace std;

template<class T>
void THLink<T>::insert(T data)
{
	THLinkNode<T> *temp = new THLinkNode<T>();
	temp->nValue = data;	//如果T没有拷贝构造函数，那么就是指向的关系
	temp->next = 0;
	if(first == 0){
		first = temp;
		current = temp;
	}else{
		current->next = temp;
		current = temp;
	}
	size++;
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
	if(first == 0){
		first = temp;
		current = temp;
	}else{
		current->next = temp;
		current = temp;
	}
	size++;
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
		size--;
		return true;
	}
	while(curr){
		if(curr->next == current){
			curr->next = 0;
			delete current;
			current = curr;
			size--;
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
	size=0;
}
template<class T>
void THLink<T>::serialize(THArchive &arc)
{
	THLinkNode<T> *curr;
	arc<<size;
	curr = first;
	while(curr){
//		arc<<curr->nValue;
		curr = curr->next;
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
	strncpy(e.key, key, strlen(key)+1);
	strncpy(e.keyValue, keyValue, strlen(keyValue)+1);
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
bool THHashTable::serialize(THArchive &arc)
{
//序列化是一个问题！如何才能更好的满足序列化的要求呢？扩展性必须高，这是肯定的了！	
	int temp = size;
	arc<<temp;
	for(int i=0; i<size; i++){
		table[i].serialize(arc);
	}
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
//	//ht.addOne("liu", "liubo1");
//	//ht.addOne("liu2", "liubo2");
//	//ht.addOne("liu3", "liubo3");
//	//ht.addOne("liu4", "liubo4");
//
//	//THArchiveWriter aw("ta");
//	//ht.serialize(aw);
//	THArchiveReader ar("ta");
//	ht.serialize(ar);
//
//	strcpy(str, ht.getOne("liu3"));
//	cout<<str;
//
//	system("pause");
//}
void fun3()
{
	guard(fun3);
	cout<<"abc";
	cout<<"def";
	unguard;
}
void fun2()
{
	guard(fun2);
	cout<<"abc";
	cout<<"def";
	throw 0;
	unguard;
}
void fun1()
{
	guard(fun1);
	fun2();
	cout<<"123";
	cout<<"456";
	throw 0;
	unguard;
}
int main()
{
	try{
		fun3();
		fun1();

	}catch(...){
		const char *ptr;
		if(CSUsed()){
			cout<<"\nException:";
			while((ptr = CSEnumFunction())){
				cout<<"-->"<<ptr;
			}
		}
	}
}