/************************************************************************/
/* 序列化接口                                                           */
/************************************************************************/
#include <iostream>
#include <fstream>
using namespace std;

class THArchive
{
public:
	virtual void serialize(void *v, int length) = 0;
	THArchive &operator << (int &data)
	{
		serialize(&data, sizeof(data));
		return const_cast<THArchive &>(*this);
	}
	THArchive &operator << (char *data)
	{
		serialize(data, strlen(data)+1);
		return const_cast<THArchive &>(*this);
	}
	//输入输出各一套
	THArchive &operator >> (char *data)
	{
		while(true){
			*data = 0;
			serialize(&*data, sizeof(char));
			if(*data++ == 0)
				break;
		}
		return const_cast<THArchive &>(*this);
	}
	THArchive &operator >> (int &data)
	{
		serialize(&data, sizeof(data));
		return const_cast<THArchive &>(*this);
	}
};
class THArchiveReader : public THArchive
{
private:
	ifstream infile;
public:
	THArchiveReader()
	{
	//	infile = 0;		//这里是错误的，该如何写？
	}
	THArchiveReader(const char *fileName)
	{
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
	
	}
	THArchiveWriter(const char *fileName)
	{
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
	void serialized(THArchive &arc, int signal = 0)
	{
		if(signal == 0)	
			arc<<name<<age;
		else
			arc>>name>>age;
	}
};