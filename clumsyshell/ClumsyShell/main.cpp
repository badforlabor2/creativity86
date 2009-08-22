//***************************
//Ŀ���Ǵ���һ��shell���򣬵������е�ʱ�򣬸ó����һֱ����
//֧�������������Ȼ����ø�������ָ���ĳ���
//Ҫ��ʹ�õ�hashtable�������ٶȿ졣
//�����ļ������xml
//clown��buffoon��joker
//***************************

#include<iostream>
#include<windows.h>
#include<tchar.h>
using namespace std;

const int LEN = 101;//������100������,101�Ǹպô���100������
TCHAR *hashTable[LEN];
int hashIndex[LEN];
byte hashCount = 0;

int FNVHash(const char *);
unsigned short hash_func(const char *str, int len);
bool putCmd(const char *key, const TCHAR *exeCmd);
TCHAR* getCmd(const char *key);
void initTable();
int main()
{
	bool b;
	initTable();
	//�������ļ��ж�ȡ���ݣ����뵽table��

	cout<<FNVHash("cmd")<<endl;
	putCmd("cmd", TEXT("cmd.exe"));
	putCmd("mat", TEXT("mat.exe"));
	putCmd("vs2005", TEXT("vs2005.exe"));
	putCmd("youdao", TEXT("youdao.exe"));
	putCmd("tm", TEXT("tm.exe"));
	putCmd("qianqian", TEXT("qianqian.exe"));
	b = getCmd("cmd");
	b = getCmd("mat");
	b = getCmd("vs2005");
	b = getCmd("youdao");
	b = getCmd("tm");
	b = getCmd("qianqian");

	system("pause");
	return 0;
}
void initTable()
{
	for(int i=0; i<LEN; i++){
		hashIndex[i] = -1;
		hashTable[i] = NULL;
	}
}
int FNVHash(const char *str)
{
	const int p=16777619;
	int hash = (int)2166136261L;
	for(int i=0; str[i]!='\0'; i++)
		hash = (hash ^ str[i]) * p;
	hash += hash << 13;
	hash ^= hash >> 7;
	hash += hash << 3;
	hash ^= hash >> 17;
	hash += hash << 5;
	return hash;
}
///�ߵ�λ����ϳ����Ľ��
unsigned short hash_func(const char *str, int len)
{
	register unsigned short sum = 0; 
	register unsigned short h = 0;
	register unsigned short *p = (unsigned short *)str;
	register unsigned short *s = (unsigned short *)str;
	while(p - s < len) 
	{
		register unsigned short a = *(p++) * (p-s);
		sum += sum ^ a;
		h += a;
	}
	return ((sum << 16) | h) % LEN;
}
///������ͻ֮��Ľ�������У���ɢ�з�������ַ��������һ�����������

bool putCmd(const char *key, const TCHAR *exeCmd)
{
	int index = hash_func(key, strlen(key)) % LEN;
	if(index >= LEN)
		return false;
	while(hashCount < LEN && hashIndex[index]!=-1){
		index ++;
		index %= LEN;
	}
	if(hashCount < LEN){
		hashCount++;
		hashIndex[index] = index;
		hashTable[index] = new TCHAR [_tcslen(exeCmd)];
		_tcscpy_s(hashTable[index], _tcslen(hashTable[index]), exeCmd);
		return true;
	}
	return false;
}
TCHAR *getCmd(const char *key)
{
	unsigned short index = hash_func(key, strlen(key));
	unsigned short i=0, j=index%LEN;
	while(i<LEN){
		if(hashIndex[j] == j)
			return hashTable[j];
		j++;
		i++;
	}
	cout<<"can't find cmd:"<<key<<endl;
	return TEXT("");
}