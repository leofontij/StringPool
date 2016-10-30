#pragma once
#include "TUnmovableDataWarehouse.h"

class TConstStringPool
{
public:
	TConstStringPool(size_t initialStrings, size_t incrementStrings, size_t maxStrings,
					 size_t initialStorage, size_t incrementStorage, size_t maxStorage);
	~TConstStringPool();
private:
	TUnmovableDataWarehouse DataWarehouse;
	const char **StringStorage;
	size_t Capacity;
	size_t Used;
	size_t MaxString;
	size_t InitialStrings;
	size_t IncrementStrings;
};

