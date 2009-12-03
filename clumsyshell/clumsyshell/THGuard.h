/************************************************************************/
/*	http://www.flipcode.com/archives/Traceable_Callstacks_with_C.shtml	*/
/*	guard宏                                                             */
/*	guard的思想就是把try...catch给宏定义话								*/
/************************************************************************/

#ifndef THGUARD_H
#define THGUARD_H
#include <vector>

vector<const char *> CallStack;
int EnumPosition = 0;
int Used = 0;
void CSAddFunction(const char *func_name)
{
	Used = 1;
	CallStack.push_back(func_name);
}
int CSUsed()
{
	return Used;
}
const char *CSEnumFunction()
{
	if(EnumPosition == CallStack.size()){
		EnumPosition = 0;
		return 0;
	}
	return CallStack[EnumPosition++];
}

#define guard(func) static const char *__FUNC_NAME__ = #func; try {
#define unguard		}catch(...){CSAddFunction(__FUNC_NAME__); throw 0; }

/************************************************************************/
/************************************************************************
例子：                                                                

void fun2()
{
	guard(fun2);
	cout<<"abc";
	cout<<"def";
	throw 0;
	unguard;
}
void fun1()
{
	guard(fun1);
	fun2();
	cout<<"123";
	cout<<"456";
	throw 0;
	unguard;
}
int main()
{
	try{
		fun1();

	}catch(...){
		const char *ptr;
		if(CSUsed()){
			cout<<"\nException:";
			while((ptr = CSEnumFunction())){
				cout<<"-->"<<ptr;
			}
		}
	}
}
************************************************************************/

#endif