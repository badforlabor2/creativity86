#include"MyHashTable.h"
#include<fstream>
using namespace std;
unsigned short MyHashTable::hash(const char *key) const
{
	register int sum = 0; 
	register int h = 0;
	register unsigned short *p = (unsigned short *)key;
	register unsigned short *s = (unsigned short *)key;
	while(p - s < strlen(key)) 
	{
		register int a = *(p++) * (p-s);
		sum += sum ^ a;
		h += a;
	}
	return unsigned short(((sum << 16) | h) % LEN);
}

bool MyHashTable::addCmd(const char *key, const char *value)
{								//���ַ��ص���ò�Ҫ��bool�����Ƿ���int���������ݲ�ͬ��int���岻ͬ�Ĵ����룬���Ժ����ԵĿ���������Ϣ
	unsigned short index, i=0, len;
	index = hash(key) % LEN;
	while(i<LEN){
		if(table[index].nCode == -1){
			table[index].nCode = index;
			len = strlen(key);
			table[index].key = new char [len+1];
			strcpy(table[index].key, key);
			len = strlen(value);
			table[index].value = new char [len+1];
			strcpy(table[index].value, value);
			return true;
		}else if(strcmp(key, table[index].key) == 0){
			cout<<"�Ѿ����˸�����"<<endl;
			return false;
		}
		index++;
		index %= LEN;
		i++;
	}
	return false;
}
int MyHashTable::findCmd(const char *key) const
{
	unsigned short index, i = 0;
	index = hash(key) % LEN;
	while(i < LEN){
		if(strcmp(table[index].key, key) == 0)
			return index;
		i++;
		index++;
		index %= LEN;
	}
	return -1;
}
char * MyHashTable::getCmd(const char *key)
{
	int index = findCmd(key);
	if(index == -1)
		return NULL;
	else 
		return table[index].value;
}
bool MyHashTable::deleteCmd(const char *key)
{
	int index = findCmd(key);
	if(index == -1){
		return false;
	}else{
		table[index].nCode = -1;
		delete table[index].key;
		delete table[index].value;
		return true;
	}

}
bool MyHashTable::renameCmd(const char *key, const char *newKey)
{
	int index = findCmd(key);
	if(index == -1){
		return false;
	}else{
		delete table[index].key;
		table[index].key = new char [strlen(newKey)+1];
		strcpy(table[index].key, newKey);
		return true;
	}
}

void MyHashTable::initTable()
{
	for(int i=0; i<LEN; i++){
		table[i].nCode = -1;
	}
}
bool MyHashTable::storeTable()
{
	return false;
}

unsigned short MyHashTable::readFromFile(const char * fileName) const
{
	ifstream inFile(fileName, ios::in|ios::binary);
	if(!inFile){
		cout<<"���ļ�ʧ��:"<<fileName<<endl;
		return 0;
	}
	int countTable = 0;
	while(!inFile.eof()){
		int nC;
		char tmp[1024];
		/*inFile>>nC;
		table[nC].nCode = nC;
		*/
		inFile.read((char *)&nC, sizeof(int));
		table[nC].nCode = nC;
		inFile.get(tmp, 1024, '\0');
		inFile.get();	//����'\0'
		table[nC].key = new char[strlen(tmp)+1];
		strcpy(table[nC].key, tmp);
		inFile.get(tmp, 1024, '\0');
		inFile.get();	//����'\0'
		cout<<"curr:"<<inFile.tellg()<<endl;
		table[nC].value = new char[strlen(tmp)+1];
		strcpy(table[nC].value, tmp);
		cout<<"�������"<<table[nC].nCode<<table[nC].key<<table[nC].value<<endl;
		countTable++;
		cout<<inFile.peek()<<endl;
		if(inFile.peek() == EOF)
			break;
	}
	cout<<"��������Ϊ"<<countTable<<endl;
	inFile.close();
	return static_cast<unsigned short>(countTable);
}

unsigned short MyHashTable::writeToFile(const char *fileName)
{														//��һ���汾���Բ���׷�ӷ�ʽ
	ofstream outFile(fileName, ios::out|ios::binary);	//������׷�ӵķ�ʽ��������tableд�뵽�ļ�
	if(!outFile){
		cout<<"�����ļ�ʧ��:"<<fileName<<endl;
		return 0;
	}
	int countTable = 0;
	for(int i=0, j; i<LEN; i++){
		if(table[i].nCode != -1){	//������������ݣ���ô���뵽�ı���
			outFile.write((char *)&table[i].nCode, sizeof(int));
			//outFile<<table[i].key;
			//outFile<<table[i].value;
			/*j = 0;
			while(j<strlen(table[i].key)+1){
				outFile.put(table[i].key[j]);
				j++;
			}
			j = 0;
			while(j<strlen(table[i].value)+1){
				outFile.put(table[i].value[j]);
				j++;
			}*/
			outFile.write(table[i].key, (strlen(table[i].key)+1)*sizeof(char) );
			outFile.write(table[i].value, (strlen(table[i].value)+1)*sizeof(char) );
			countTable++;	
		}
	}	
	cout<<"��¼����Ϊ��"<<countTable<<endl;
	outFile.close();
	return static_cast<unsigned short>(countTable);
	
}

MyHashTable::MyHashTable(const char *fileName)
{
	initTable();
	this->fileName = new char [strlen(fileName)+1];
	strcpy(this->fileName, fileName);
	this->readFromFile(this->fileName);
}
void MyHashTable::saveTable()
{
	this->writeToFile(this->fileName);
}