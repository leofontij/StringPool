#include "stdafx.h"
#include "TUnmovableDataWarehouse.h"
#include <stdlib.h>
#include <math.h>


TUnmovableDataWarehouse::TUnmovableDataWarehouse(size_t initialSize, size_t incrementSize, size_t maxSize, size_t maxWasted, size_t alignment)
	: InitialSize(RoundUp(initialSize, alignment))
	, IncrementSize(RoundUp(incrementSize, alignment))
	, MaxSize(RoundUp(maxSize, alignment))
	, MaxWasted(RoundUp(maxWasted, alignment))
	, Alignment(alignment)
	, NumberOfDepartments(0)
	, WarehouseDepartments(NULL)
	, NumberOfWarehouseDepartments(0)
	, FirstNonFullDepartPartment(0)
	, TotalCapacity(0)
	, TotalUsed(0)
{
}


TUnmovableDataWarehouse::~TUnmovableDataWarehouse()
{
	// No de partments, just return
	if (WarehouseDepartments == NULL)
		return;

	// Delete every department
	while (NumberOfWarehouseDepartments != 0)
	{
		NumberOfWarehouseDepartments--;
		delete WarehouseDepartments[NumberOfWarehouseDepartments];
		WarehouseDepartments[NumberOfWarehouseDepartments] = NULL;
	}
	// Delete the warehouse
	free(WarehouseDepartments);
}

size_t TUnmovableDataWarehouse::RoundUp(size_t size, size_t alignment)
{
	size_t retVal = (size / alignment) * alignment;
	if (size > retVal)
	{
		retVal += alignment;
	}
	return retVal;
}

void * TUnmovableDataWarehouse::StoreData(void * data, size_t dataSize)
{
	size_t roundedDataSize = RoundUp(dataSize, Alignment);
	// If a maximum total size was specified
	if (MaxSize > 0)
	{
		if ((TotalUsed + roundedDataSize) > MaxSize)
		{
			return NULL;
		}
	}

	void * retVal = NULL;
	int departmentIx;
	departmentIx = FirstNonFullDepartPartment;
	
	while (true)
	{
		if (departmentIx >= NumberOfWarehouseDepartments)
		{
			size_t newDepartmentSize = (NumberOfWarehouseDepartments == 0) ?
				__max(InitialSize, roundedDataSize) :
				__max(IncrementSize, roundedDataSize);
			NumberOfWarehouseDepartments++;
			WarehouseDepartments = (TWarehouseDepartment **)realloc(WarehouseDepartments, NumberOfWarehouseDepartments * sizeof(*WarehouseDepartments));
			WarehouseDepartments[NumberOfWarehouseDepartments] = new TWarehouseDepartment(newDepartmentSize, MaxWasted, Alignment);
			TotalCapacity += newDepartmentSize;
		}

		TWarehouseDepartment * pCurrentDepartment = WarehouseDepartments[departmentIx];
		retVal = pCurrentDepartment->StoreData(data, dataSize);
		if (retVal != NULL)
		{
			return retVal;
		}

		if (pCurrentDepartment->IsFull())
		{
			if (departmentIx == FirstNonFullDepartPartment)
			{
				FirstNonFullDepartPartment++;
			}
		}
		departmentIx++;
	}
}


TUnmovableDataWarehouse::TWarehouseDepartment::TWarehouseDepartment(size_t capacity, size_t maxWasted, size_t alignment)
: Capacity(TUnmovableDataWarehouse::RoundUp(capacity, alignment))
, MaxWasted(TUnmovableDataWarehouse::RoundUp(maxWasted,alignment))
, Alignment(alignment)
, Used(0)
{
	if (capacity > 0)
	{
		StorageArea = new char[Capacity];
	}
	else
	{
		StorageArea = NULL;
	}
}

TUnmovableDataWarehouse::TWarehouseDepartment::~TWarehouseDepartment()
{
	if (StorageArea != NULL)
	{
		delete[] StorageArea;
		StorageArea = NULL;
	}
	Capacity = 0;
	Used = 0;

}

void * TUnmovableDataWarehouse::TWarehouseDepartment::StoreData(void * data, size_t dataSize)
{
	size_t offset = TUnmovableDataWarehouse::RoundUp(Used, Alignment);
	if ((offset + dataSize) > Capacity)
	{
		return NULL;
	}
	memcpy(&StorageArea[offset], data, dataSize);
	Used = offset + dataSize;
	return &StorageArea[offset];
}
	
bool TUnmovableDataWarehouse::TWarehouseDepartment::IsFull()
{
	size_t unused = Capacity - Used;
	return (unused <= MaxWasted);
}
