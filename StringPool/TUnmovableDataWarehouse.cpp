#include "stdafx.h"
#include "TUnmovableDataWarehouse.h"
#include <stdlib.h>
#include <math.h>

#define DATAWAREHOUSE_VERBOSE

namespace lefoso {
	namespace system {

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

		/**********************************************************************************************//**
		 * \fn	TUnmovableDataWarehouse::~TUnmovableDataWarehouse()
		 *
		 * \brief	Destructor.
		 *
		 * Deletes all the departments in the warehouse. References to data stored in the warehouse
		 * will probably cause serious errors. So the warehouse should be deleted as one of the last items 
		 * in its context.
		 * 
		 * \author	Leo
		 * \date	2016-11-26
		 **************************************************************************************************/

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

		/**********************************************************************************************//**
		 * \fn	size_t TUnmovableDataWarehouse::RoundUp(size_t size, size_t alignment)
		 *
		 * \brief	Round up. If required rounds up a size as multiple of alignment
		 *
		 * \author	Leo
		 * \date	2016-11-26
		 *
		 * \param	size	 	The size.
		 * \param	alignment	The alignment.
		 *
		 * \return	A size_t wich is a multiple of alignment.
		 **************************************************************************************************/

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
#ifdef DATAWAREHOUSE_VERBOSE
			printf("Storing %u bytes in DataWarehouse.\r\n", dataSize);
#endif

			size_t roundedDataSize = RoundUp(dataSize, Alignment);
			// If a maximum total size was specified
			if (MaxSize > 0)
			{
				if ((TotalUsed + roundedDataSize) > MaxSize)
				{
#ifdef DATAWAREHOUSE_VERBOSE
					printf("DataWarehouse capacity exceeded.\r\n");
#endif
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
#ifdef DATAWAREHOUSE_VERBOSE
					printf("Creating new department %u for %u bytes.\r\n", NumberOfWarehouseDepartments - 1, newDepartmentSize);
#endif

					WarehouseDepartments[NumberOfWarehouseDepartments - 1] = new TWarehouseDepartment(newDepartmentSize, MaxWasted, Alignment);
					TotalCapacity += newDepartmentSize;
				}

				TWarehouseDepartment * pCurrentDepartment = WarehouseDepartments[departmentIx];
				retVal = pCurrentDepartment->StoreData(data, dataSize);
				if (retVal != NULL)
				{
					TotalUsed += dataSize;
					return retVal;
				}
#ifdef DATAWAREHOUSE_VERBOSE
				printf("Department %u rejected data due to lack of space.\r\n", departmentIx);
#endif


				if (pCurrentDepartment->IsFull())
				{
					if (departmentIx == FirstNonFullDepartPartment)
					{
#ifdef DATAWAREHOUSE_VERBOSE
						printf("Declaring department %u to be full.\r\n", departmentIx);
#endif
						FirstNonFullDepartPartment++;
					}
				}
				departmentIx++;
			}
		}


		TUnmovableDataWarehouse::TWarehouseDepartment::TWarehouseDepartment(size_t capacity, size_t maxWasted, size_t alignment)
			: Capacity(TUnmovableDataWarehouse::RoundUp(capacity, alignment))
			, MaxWasted(TUnmovableDataWarehouse::RoundUp(maxWasted, alignment))
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

	}
}