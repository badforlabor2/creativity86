#include "platform.h"
#include "THArchive.h"

BOOL gIsConsoleCookie = 0;

THArchive &THArchive::ByteOrderSerialize(void *v, INT size)
{
#if __INTEL_BYTE_ORDER__
	BOOL SwapBytes = gIsConsoleCookie && ArIsPersistent;
#else	
	BOOL SwapBytes = ArIsPersistent;
#endif
	if(SwapBytes){
		//内存和文件之间的互相转化
		for(INT i=size-1; i>=0; i--){
			Serialize((BYTE*)v + i, 1);
		}
	}else{
		Serialize(v, size);
	}
	return *this;

}