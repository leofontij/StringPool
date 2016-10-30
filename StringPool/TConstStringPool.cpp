#include "stdafx.h"
#include "TConstStringPool.h"


TConstStringPool::TConstStringPool(	size_t initialStrings, size_t incrementStrings, size_t maxStrings,
									size_t initialStorage, size_t incrementStorage, size_t maxStorage)
: DataWarehouse(initialStorage, incrementStorage, maxStorage)
, StringStorage(NULL)
, Capacity(0)
, Used(0)
, MaxString(maxStrings)
, InitialStrings()
, IncrementStrings()
{
}


TConstStringPool::~TConstStringPool()
{
	if (StringStorage != NULL)
	{
		delete[] StringStorage;
		StringStorage = NULL;
	}
	Capacity = 0;
	Used = 0;
}
