/************************************************************************/
/* 序列化接口                                                           */
/************************************************************************/
#ifndef THARCHIVE_H
#define THARCHIVE_H

#include <iostream>
#include <fstream>
using namespace std;

enum iotype
{
	OUTPUT,
	INPUT,
	NONE,
};
class THArchive
{
protected:
	iotype type;
public:
	virtual void serialize(void *v, int length) = 0;
	THArchive():type(NONE)
	{
	
	}
	//还有一种方法，由子类去实现"<<"运算符，这样或许更好
	THArchive &operator << (int &data)
	{
		serialize(&data, sizeof(data));
		return const_cast<THArchive &>(*this);
	}
	THArchive &operator << (char *data)
	{
		if(type == OUTPUT){
			serialize(data, strlen(data)+1);
		}else if(type == INPUT){
			//对于while循环最好加一个守护代码，比如i<100000，这样防止程序进入死循环
			while(true){
				*data = 0;
				serialize(&*data, sizeof(char));
				if(*data++ == 0)
					break;
			}
		}
		return const_cast<THArchive &>(*this);
	}
	//输入输出各一套
	//THArchive &operator >> (char *data)
	//{
	//	while(true){
	//		*data = 0;
	//		serialize(&*data, sizeof(char));
	//		if(*data++ == 0)
	//			break;
	//	}
	//	return const_cast<THArchive &>(*this);
	//}
	//THArchive &operator >> (int &data)
	//{
	//	serialize(&data, sizeof(data));
	//	return const_cast<THArchive &>(*this);
	//}
};
class THArchiveReader : public THArchive
{
private:
	ifstream infile;
public:
	THArchiveReader()
	{
		type = INPUT;
	//	infile = 0;		//这里是错误的，该如何写？
	}
	THArchiveReader(const char *fileName)
	{
		this->type = INPUT;
		infile.open(fileName);
	}
	bool open(const char *fileName)
	{
		infile.open(fileName, ios::in | ios::binary);
		return (infile.is_open());	//不能使用infile != 0，因为infile不是指针！
	}
	virtual void serialize(void *v, int length)
	{
		if(infile.is_open())
			infile.read((char *)v, length);
	}
	~THArchiveReader()
	{
		//执行关闭或者没有打开的时候infile肯定值为0
		if(infile.is_open())
			infile.close();
	}
};
class THArchiveWriter : public THArchive
{
private:
	ofstream outfile;
public:
	THArchiveWriter()
	{
		type = OUTPUT;
	}
	THArchiveWriter(const char *fileName)
	{
		type = OUTPUT;
		outfile.open(fileName);
	}
	bool open(const char *fileName)
	{
		outfile.open(fileName);
		return (outfile.is_open());
	}
	void serialize(void *v, int length)
	{
		if(outfile.is_open())
			outfile.write(static_cast<char *>(v), length);
	}
	~THArchiveWriter()
	{
		if(outfile.is_open())
			outfile.close();
		if(outfile.is_open())
			cout<<"crash!"<<endl;
	}
};
class THTestArchive
{
public:
	char name[10];
	int age;
	THTestArchive()
	{
		name[0] = 0;
	}
	void serialized(THArchive &arc)
	{
		arc<<name<<age;
	}
};
#endif