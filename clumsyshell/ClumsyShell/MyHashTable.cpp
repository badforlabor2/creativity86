#include"MyHashTable.h"

unsigned short MyHashTable::hash(const char *key)
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

bool MyHashTable::addCmd(const TCHAR *key, const TCHAR *value)
{
	unsigned short index, i=0, len;
	char keybyte[1024];
	if(!WideCharToMultiByte(0, 0, key, _tcslen(key), keybyte, 1024, NULL, NULL))
		return false;
	index = hash(keybyte) % LEN;
	while(i<LEN){
		if(table[index].nCode != -1){
			table[index].nCode = index;
			len = _tcslen(key);
			table[index].key = new TCHAR [len];
			_tcscpy_s(table[index].key, len * sizeof(TCHAR), key);
			len = _tcslen(value);
			table[index].key = new TCHAR [len];
			_tcscpy_s(table[index].value, len * sizeof(TCHAR), value);
			return true;
		}
		index++;
		index %= LEN;
		i++;
	}
	return false;
}

TCHAR * MyHashTable::getCmd(const TCHAR *key)
{
	unsigned short index, i=0;
	char keybyte[1024];
	if(!WideCharToMultiByte(0, 0, key, _tcslen(key), keybyte, 1024, 0, 0))
		return TEXT("");
	index = hash(keybyte) % LEN;
	while(i<LEN){
		if(_tcscmp(table[index].key, key) == 0){
			return table[index].value;
		}
		i++;
		index++;
		index %= LEN;
	}
	return TEXT("");
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

unsigned short MyHashTable::readFromFile(const TCHAR * fileName)
{
	return 0;
}

unsigned short MyHashTable::writeToFile(const TCHAR *fileName)
{
	return 0;
}

MyHashTable::MyHashTable(const TCHAR *fileName)
{
		
}