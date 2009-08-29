//构造函数中输入文件名字，之后调用addCmd或者getCmd即可
//c++也能识别中文，只是编码问题的差异，所以以后底层均用ansi c++写
//当用到某个类、函数的时候再用windows转码
#include<iostream>

//创建一个简单的hash表
typedef struct _Table   
{
	int nCode;
	char *key;
	char *value;
} Table;

class MyHashTable{
static const int LEN = 101;	//存放101条命令,101是质数
private :
	mutable Table table[LEN];
	char *fileName;
public:
	MyHashTable(){}
	MyHashTable(const char *fileName);
	unsigned short hash(const char *) const;	//转化hash码
	bool addCmd(const char *, const char *);	//加入key-value
	int findCmd(const char *) const;	//找到索引
	char *getCmd(const char *);	//获得key对应的键值
	bool deleteCmd(const char *);	//删除
	bool renameCmd(const char *, const char *newKey);
	unsigned short readFromFile(const char *) const;	//从文件中获得table
	unsigned short writeToFile(const char *);	//把table写入到文件中
	void initTable();					//初始化table值
	bool storeTable();
	void inline copyStruct(Table &t1, const Table& t2) const {
		t1.nCode = t2.nCode;
		t1.key = t2.key;
		t1.value = t2.value;
	}
	void saveTable();
};
