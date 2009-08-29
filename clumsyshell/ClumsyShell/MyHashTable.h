//���캯���������ļ����֣�֮�����addCmd����getCmd����
//c++Ҳ��ʶ�����ģ�ֻ�Ǳ�������Ĳ��죬�����Ժ�ײ����ansi c++д
//���õ�ĳ���ࡢ������ʱ������windowsת��
#include<iostream>

//����һ���򵥵�hash��
typedef struct _Table   
{
	int nCode;
	char *key;
	char *value;
} Table;

class MyHashTable{
static const int LEN = 101;	//���101������,101������
private :
	mutable Table table[LEN];
	char *fileName;
public:
	MyHashTable(){}
	MyHashTable(const char *fileName);
	unsigned short hash(const char *) const;	//ת��hash��
	bool addCmd(const char *, const char *);	//����key-value
	int findCmd(const char *) const;	//�ҵ�����
	char *getCmd(const char *);	//���key��Ӧ�ļ�ֵ
	bool deleteCmd(const char *);	//ɾ��
	bool renameCmd(const char *, const char *newKey);
	unsigned short readFromFile(const char *) const;	//���ļ��л��table
	unsigned short writeToFile(const char *);	//��tableд�뵽�ļ���
	void initTable();					//��ʼ��tableֵ
	bool storeTable();
	void inline copyStruct(Table &t1, const Table& t2) const {
		t1.nCode = t2.nCode;
		t1.key = t2.key;
		t1.value = t2.value;
	}
	void saveTable();
};
