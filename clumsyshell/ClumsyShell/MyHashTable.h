//构造函数中输入文件名字，之后调用addCmd或者getCmd即可

#include<iostream>
#include<windows.h>
#include<tchar.h>

//创建一个简单的hash表
typedef struct _Table   
{
	int nCode;
	TCHAR *key;
	TCHAR *value;
} Table;

class MyHashTable{
static const int LEN = 101;	//存放101条命令,101是质数
private :
	Table table[LEN];
	TCHAR *fileName;
public:
	MyHashTable(){}
	MyHashTable(const TCHAR *fileName);
	unsigned short hash(const char *);	//转化hash码
	bool addCmd(const TCHAR *, const TCHAR *);	//加入key-value
	TCHAR *getCmd(const TCHAR *);	//获得key对应的键值
	unsigned short readFromFile(const TCHAR *);	//从文件中获得table
	unsigned short writeToFile(const TCHAR *);	//把table写入到文件中
	void initTable();					//初始化table值
	bool storeTable();
};
