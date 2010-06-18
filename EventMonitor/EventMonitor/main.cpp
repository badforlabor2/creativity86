#include <iostream>
#include "eventmonitor.h"

using namespace std;

int main()
{
	AExample ae;
	//ae.show();

	THEventSet es;
	es.subscribeEvent(0, THSubscriber(&AExample::SayHello, &ae));
	THEventArgs eargs;
	es.fireEvent(0, eargs);
	es.removeEvent(0);
	system("pause");
	return 0;
}