/************************************************************************/
/* liubo, 定义归档类，将信息保存到文件中                                */
/************************************************************************/
#include<iostream>
#include<fstream>
using namespace std;

class MyString
{
public:
	MyString()
		: pData(0), size(0)
	{
	
	}
	MyString(const char* chs)
	{
		size = strlen(chs)+1;
		AddEmpty(size);
		for(int i=0; i<size; i++)
		{
			GetPtr()[i] = chs[i];
		}
	}
	~MyString()
	{
		if(size > 0)
			delete pData;
		pData = NULL;
		size = 0;
	}
	void AddEmpty(int cnt)
	{
		size = cnt;
		if(size > 0)
			pData = new char[size];
	}
	char* GetPtr()
	{
		return pData;
	}
	int GetSize()
	{
		return size;
	}
private:
	char *pData;
	int	size;
};

class Archives
{
public:
	virtual void serialize(void *v, int length) = 0;
	virtual void flush() {};

	Archives &operator << (int &data){
		serialize(&data, sizeof(data));
		return const_cast<Archives &>(*this);
	}
	friend Archives& operator<<(Archives& ar, float& v);
	friend Archives& operator<<(Archives& ar, char& v);
	bool IsLoad() {return type==0;}
protected:
	int type;	//0表示load/read，1表示save/write
};
Archives& operator<<(Archives& ar, MyString& str);

class ArchivesReader : public Archives
{
private:
	ifstream in;
public:
	ArchivesReader(const char *fileName)
		:in(fileName, ios::in|ios::binary/*|ios::app*/)
	{
		type = 0;		
	}
	virtual void serialize(void *v, int length){
		//从文件读取
		in.read((char *)v, length);
	}	
	void flush()
	{
		in.close();
	}
};

class ArchivesWrite : public Archives
{
private:
	ofstream out;
public:
	ArchivesWrite(const char *fileName)
		:out(fileName, ios::out|ios::binary/*|ios::app*/)
	{
		type = 1;
	}
	virtual void serialize(void *v, int length)
	{
		//输出到文件	
		out.write((char *)v, length);
	}
	void flush()
	{
		out.close();
	}
};

typedef std::basic_string<unsigned char> MyMem;

class MemoryWriter : public Archives
{
public:
	MemoryWriter() : pos(0) {type = 1;}

	virtual void serialize(void *v, int length)
	{
		serialize((unsigned char*)v, length);
	}
	void serialize(unsigned char *v, int length)
	{
		for(int i=0; i<length; i++)
			mem.push_back(v[i]);
		pos += length;
	}
	const MyMem& GetMyMem() const
	{
		return mem;
	}
private:
	MyMem mem;
	int pos;
};

class MemoryReader : public Archives
{
public:
	MemoryReader(const MyMem& in)
		:mem(in), pos(in.length())
	{
		type = 0;
	}
	virtual void serialize(void *v, int length)
	{
		serialize((unsigned char*)v, length);
	}
	void serialize(unsigned char *v, int length)
	{
		if(pos < length || pos <= 0)
			return;
		for(int i=0; i<length; i++)
			v[i] = mem[i];
		pos -= length;
		mem.erase(0, length);
	}
private:
	MyMem mem;
	int pos;
};







