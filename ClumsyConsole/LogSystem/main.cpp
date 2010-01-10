
#include "platform.h"
#include "THFileManagerWindows.h"
#include "THOutputDeviceFile.h"

THFileManagerWindows WindowsFileManager;
THFileManager *gFileManager = &WindowsFileManager;
HINSTANCE hInstance;

int main()
{
	THOutputDeviceFile Log;
	thStrcpy(Log.FileName, TEXT("Sample.txt"));
	Log.Serialize(TEXT("ab¡ı≤©cd"));
	Log.Serialize(TEXT("----ab¡ı≤©cd----"));
	Log.Flush();

	system("pause");
	return 0;
}