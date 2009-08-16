#include<iostream>
#include<fstream>
#include<stack>
using namespace std;

bool analyzeLine(char *, const int, int&);
bool checkCSV(char *);
bool checkDir(char *);	//目录操作，目录与平台有关，所以这个暂时不做

//换行符为：0x0d+0x0a，‘,’为0x2c，结束符为EOF
const int MAX=15000;

int main(int argc, char *argv[])
{
	//如果没有参数
	if(argc == 1){
		cout<<"请输入一个csv表(必须带上扩展名字.csv)"<<endl;
		return 0;
	}else if(argc == 2){
		//检索*.csv
		if(argv[1] == "*.csv"){
				
		}else{
			//如果要检索一个csv
			if(checkCSV(argv[1]))
				cout<<"Successful In "<<argv[1]<<endl;
			else
				cout<<"Failed In "<<argv[1]<<endl;
		}
	}
	//如果要检索多个csv
	

	system("pause");
	return 0;
}

///读取一行数据
void readLine(ifstream file, char * line)
{

}

///检查该目录下所有的CSV数据
bool checkDir(char *dir)
{
	return false;
}

///检查一个CSV表
bool checkCSV(char *fileName)
{
	ifstream file;
	file.open(fileName, ios::out|ios::binary);
	if(file == NULL){
		cout<<"无法打开文件:"<<fileName<<endl;
		return false;
	}
	char c;
	char szline[MAX];
	int nline = 0, index = 0;
	int nComma, j;
	while(!file.eof()){
		file.get(c);
		//换行为'ef'
		if(c == 0x0d){
			cout<<"获得一个'\\n\\r' ";
			file.get(c);
			if(c == 0x0a){
				nline++;
				cout<<"您获取了第<"<<nline<<">行数据"<<endl;
				szline[index++] = '\0';
				if(!analyzeLine(szline, index, j)){
					cout<<"###### ERROR ###:"<<nline<<endl;
					return false;
				}
				if(nline == 1){
					nComma = j;
				}else if(nComma != j){
					cout<<"## ERROR ## 第"<<nline<<"行数据不符合要求！表头<"<<nComma<<">个而此行<"<<j<<">个\n";
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
		cout<<"没有获得'\\r\\n'，但是"<<"您获取了第<"<<nline<<">行数据"<<endl;
		szline[index++] = '\0';
		if(!analyzeLine(szline, index, j)){
			cout<<"###### ERROR ###:"<<nline<<endl;
			cout<<"failed!"<<endl;
			return false;
		}
		if(nline == 1){
			nComma = j;
			cout<<">>>>表头共<"<<nComma<<">个";
		}else if(nComma != j){
			cout<<"## ERROR ## 第"<<nline<<"行数据不符合要求！表头<"<<nComma<<">个而此行<"<<j<<">个\n";
			return false;
		}
		index = 0;
		szline[index] = '\0';
	}

	cout<<"analyse successfully! total lines:"<<nline<<endl;
	return true;
}

///解析一行数据
bool analyzeLine(char *line, const int len, int& nComma)
{
	//按照编码规则，逐个字符检测
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
				//按照csv的规则，如果是"""这种情况
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

