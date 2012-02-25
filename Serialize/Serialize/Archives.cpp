/************************************************************************/
/* liubo, 定义归档类，将信息保存到文件中                                */
/* .cpp																	*/
/************************************************************************/
#include "Archives.h"

Archives& operator<<( Archives& ar, float& v)
{
	ar.serialize(&v, sizeof(float));
	return ar;
}
Archives& operator<<(Archives& ar, char& v)
{
	ar.serialize(&v, sizeof(char));
	return ar;
}

Archives& operator<<(Archives& ar, MyString& str)
{
	if(ar.IsLoad())
	{
		int size = 0;
		ar<<size;
		str.AddEmpty(size);
		for(int i=0; i<str.GetSize(); i++)
		{
			ar<<str.GetPtr()[i];
		}
	}
	else
	{
		int size = str.GetSize();
		ar<<size;
		for(int i=0; i<str.GetSize(); i++)
		{
			ar<<str.GetPtr()[i];
		}
	}
	return ar;
}