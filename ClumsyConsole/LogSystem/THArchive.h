/************************************************************************/
/* 序列化接口                                                           */
/************************************************************************/
#ifndef THARCHIVE_H
#define THARCHIVE_H

const INT INDEX_NONE = -1;
#define ARRAY_COUNT( array ) \
	( sizeof(array) / sizeof((array)[0]) )

class THArchive
{
protected:
	INT ArVer;
	BOOL ArIsLoading;
	BOOL ArIsSaving;
	INT Stoper;
	BOOL ArIsPersistent;
	BOOL ArIsError;
public:
	virtual ~THArchive() {}
	virtual void Serialize(void *v, INT size) {}
	virtual void SerializeBits(void *v, INT size)
	{
		Serialize(v, (size+7)/8);
		//if(IsLoading()){
		//	(TCHAR *)v[size/8] &= ((1<<size&7) - 1);
		//}
	}
	virtual void SerializeInt(void *v, INT size)
	{
		ByteOrderSerialize(v, size);
	}
	THArchive &ByteOrderSerialize(void *v, INT size);
	virtual INT Tell()
	{
		return INDEX_NONE;
	}
	virtual INT TotalSize()
	{
		return INDEX_NONE;
	}
	virtual BOOL AtEnd()
	{
		INT Pos = Tell();
		return Pos!=INDEX_NONE && Pos>=TotalSize();
	}
	virtual void Seek(INT pos) {}
	virtual void PreCache(INT hintCount) {}
	virtual void Flush() {}
	virtual BOOL Close() 
	{
		return !ArIsError;
	}
	virtual BOOL GetError() 
	{
		return ArIsError;
	}
	BOOL IsLoading()
	{
		return ArIsLoading;
	}
	BOOL IsSaving()
	{
		return ArIsSaving;
	}
	BOOL IsError()
	{
		return ArIsError;
	}
	THArchive()
		:	ArIsError(0), ArIsLoading(0), ArIsSaving(0),
			Stoper(0), ArVer(INDEX_NONE)
	{
	
	}
};




#endif