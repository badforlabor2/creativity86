/************************************************************************/
/* liubo, 定义归档类，将信息保存到文件中                                */
/************************************************************************/
#include<iostream>
#include<fstream>
using namespace std;

class Archives{
public:
	virtual void serialize(void *v, int length) = 0;
	Archives &operator << (int &data){
		serialize(&data, sizeof(data));
		return const_cast<Archives &>(*this);
	}
};

class ArchivesReader : public Archives{
private:
	char *file;
public:
	ArchivesReader(const char *fileName)
	{
		file = new char [strlen(fileName)+1];
		strcpy(file, fileName);
	}
	virtual void serialize(void *v, int length){
		//从文件读取
		ifstream in(file, ios::in|ios::binary);
		in.read((char *)v, length);
		in.close();
	}	
};

class ArchivesWrite : public Archives{
private:
	char *file;
public:
	ArchivesWrite(const char *fileName)
	{
		file = new char [strlen(fileName)+1];
		strcpy(file, fileName);
	}
	virtual void serialize(void *v, int length)
	{
		//输出到文件	
		ofstream out(file, ios::out|ios::binary);
		out.write((char *)v, length);
		out.close();
	}
};