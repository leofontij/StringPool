// StringPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TConstStringPool.h"
using namespace lefoso::system;

int main()
{
	TConstStringPool pool(10, 5, 0, 1024, 512, 0);
	for (int i = 0; i < 10000; i++)
	{
		char buffer[100];
		sprintf_s(buffer, sizeof(buffer), "MyText %d", i % 1000);
		const char * result = pool.AddString(buffer);
		printf("%d %s\r\n", i, result);

	}
    return 0;
}

