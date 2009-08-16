#include<iostream>
#include<fstream>
#include<stack>
using namespace std;

bool analyzeLine(char *, const int, int&);
bool checkCSV(char *);
bool checkDir(char *);	//Ŀ¼������Ŀ¼��ƽ̨�йأ����������ʱ����

//���з�Ϊ��0x0d+0x0a����,��Ϊ0x2c��������ΪEOF
const int MAX=15000;

int main(int argc, char *argv[])
{
	//���û�в���
	if(argc == 1){
		cout<<"������һ��csv��(���������չ����.csv)"<<endl;
		return 0;
	}else if(argc == 2){
		//����*.csv
		if(argv[1] == "*.csv"){
				
		}else{
			//���Ҫ����һ��csv
			if(checkCSV(argv[1]))
				cout<<"Successful In "<<argv[1]<<endl;
			else
				cout<<"Failed In "<<argv[1]<<endl;
		}
	}
	//���Ҫ�������csv
	

	system("pause");
	return 0;
}

///��ȡһ������
void readLine(ifstream file, char * line)
{

}

///����Ŀ¼�����е�CSV����
bool checkDir(char *dir)
{
	return false;
}

///���һ��CSV��
bool checkCSV(char *fileName)
{
	ifstream file;
	file.open(fileName, ios::out|ios::binary);
	if(file == NULL){
		cout<<"�޷����ļ�:"<<fileName<<endl;
		return false;
	}
	char c;
	char szline[MAX];
	int nline = 0, index = 0;
	int nComma, j;
	while(!file.eof()){
		file.get(c);
		//����Ϊ'ef'
		if(c == 0x0d){
			cout<<"���һ��'\\n\\r' ";
			file.get(c);
			if(c == 0x0a){
				nline++;
				cout<<"����ȡ�˵�<"<<nline<<">������"<<endl;
				szline[index++] = '\0';
				if(!analyzeLine(szline, index, j)){
					cout<<"###### ERROR ###:"<<nline<<endl;
					return false;
				}
				if(nline == 1){
					nComma = j;
				}else if(nComma != j){
					cout<<"## ERROR ## ��"<<nline<<"�����ݲ�����Ҫ�󣡱�ͷ<"<<nComma<<">��������<"<<j<<">��\n";
					return false;
				}
				index = 0;
				szline[index] = '\0';
			}
		}else{
			szline[index++] = c;
		}
	}
	if(index > 0){
		nline++;
		cout<<"û�л��'\\r\\n'������"<<"����ȡ�˵�<"<<nline<<">������"<<endl;
		szline[index++] = '\0';
		if(!analyzeLine(szline, index, j)){
			cout<<"###### ERROR ###:"<<nline<<endl;
			cout<<"failed!"<<endl;
			return false;
		}
		if(nline == 1){
			nComma = j;
			cout<<">>>>��ͷ��<"<<nComma<<">��";
		}else if(nComma != j){
			cout<<"## ERROR ## ��"<<nline<<"�����ݲ�����Ҫ�󣡱�ͷ<"<<nComma<<">��������<"<<j<<">��\n";
			return false;
		}
		index = 0;
		szline[index] = '\0';
	}

	cout<<"analyse successfully! total lines:"<<nline<<endl;
	return true;
}

///����һ������
bool analyzeLine(char *line, const int len, int& nComma)
{
	//���ձ����������ַ����
	//0000 - 007F       0xxxxxxx 
	//0080 - 07FF       110xxxxx 10xxxxxx 
	//0800 - FFFF       1110xxxx 10xxxxxx 10xxxxxx 
	int i, j=0;
	stack<char> quotation;
	char c;
	for(i=0; i<len; i++){
		c = line[i];
		if((c & 0xe0) == 0xe0){	//0800~
			if((i+1 >= len) || (i+2 >=len))
				return false;
			c = line[++i];
			if((c & 0x80) != 0x80)
				return false;
			c = line[++i];
			if((c & 0x80) != 0x80)
				return false;
		}else if((c & 0xb0) == 0xb0){	//0080~
			if(i+1 >= len)
				return false;
			c = line[++i];
			if((c & 0x80) != 0x80)
				return false;
		}else if((c & 0x00) == 0x00){	//0000~
			if(c == '"'){
				//����csv�Ĺ��������"""�������
				if(quotation.empty() && (i+1 < len) && (i+2 < len)
					&& line[i+1] == '"' && line[i+2] == '"'){
					i += 2;	
				}else{
					if(quotation.empty())
						quotation.push('"');
					else
						quotation.pop();
				}
			}else if(c == ',' && quotation.empty())
				j++;
		}
	}
	nComma = j;
	return true;
}

