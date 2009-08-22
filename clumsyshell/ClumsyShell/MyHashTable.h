//���캯���������ļ����֣�֮�����addCmd����getCmd����

#include<iostream>
#include<windows.h>
#include<tchar.h>

//����һ���򵥵�hash��
typedef struct _Table   
{
	int nCode;
	TCHAR *key;
	TCHAR *value;
} Table;

class MyHashTable{
static const int LEN = 101;	//���101������,101������
private :
	Table table[LEN];
	TCHAR *fileName;
public:
	MyHashTable(){}
	MyHashTable(const TCHAR *fileName);
	unsigned short hash(const char *);	//ת��hash��
	bool addCmd(const TCHAR *, const TCHAR *);	//����key-value
	TCHAR *getCmd(const TCHAR *);	//���key��Ӧ�ļ�ֵ
	unsigned short readFromFile(const TCHAR *);	//���ļ��л��table
	unsigned short writeToFile(const TCHAR *);	//��tableд�뵽�ļ���
	void initTable();					//��ʼ��tableֵ
	bool storeTable();
};
