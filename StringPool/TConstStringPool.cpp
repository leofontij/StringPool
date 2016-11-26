#include "stdafx.h"
#include "TConstStringPool.h"
#include <string.h>
#include <stdlib.h>

#define STRINGPOOL_VERBOSE

namespace lefoso
{
	namespace system
	{
		TConstStringPool::TConstStringPool(size_t initialStrings, size_t incrementStrings, size_t maxStrings,
			size_t initialStorage, size_t incrementStorage, size_t maxStorage)
			: DataWarehouse(initialStorage, incrementStorage, maxStorage, 80, sizeof(char))
			, StringStorage(NULL)
			, Capacity(0)
			, Used(0)
			, MaxStrings(maxStrings)
			, InitialStrings(initialStrings)
			, IncrementStrings(incrementStrings)
			, IsSorted(false)
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

		const char * TConstStringPool::AddString(const char * s)
		{
			const char * retVal = FindString(s);
			if (retVal != NULL)
			{
				return retVal;
			}
			return AddNewString(s);
		}

		const char * TConstStringPool::FindString(const char * s)
		{
			if (StringStorage == NULL)
			{
				return NULL;
			}
			if (!IsSorted)
			{
				Sort();
			}
			void * p = bsearch(&s, StringStorage, Used, sizeof(*StringStorage), TConstStringPool::fsort);
			if (p != NULL)
			{
				return *((char**)p);
			}
			return NULL;
		}

		int __cdecl TConstStringPool::fsort(const void * p1, const void * p2)
		{
			return strcmp(*((char **)p1), *((char **)p2));
		}


		void TConstStringPool::Sort()
		{
			qsort(StringStorage, Used, sizeof(*StringStorage), TConstStringPool::fsort);
			IsSorted = true;
		}

		const char * TConstStringPool::AddNewString(const char * s)
		{
#ifdef STRINGPOOL_VERBOSE
			printf("Adding new string '%s'\r\n", s);
#endif
			if (MaxStrings > 0)
			{
				if (Used >= MaxStrings)
				{
#ifdef STRINGPOOL_VERBOSE
					printf("Maximum numbers of strings exceeded \r\n");
#endif
					return NULL;
				}
			}
			if (Used >= Capacity)
			{
				size_t inc = (StringStorage == NULL) ? InitialStrings : IncrementStrings;
				Capacity += inc;
#ifdef STRINGPOOL_VERBOSE
				printf("Extending string storage to %u strings \r\n", Capacity);
#endif
				StringStorage = (const char **)realloc(StringStorage, Capacity * sizeof(*StringStorage));
			}
			const char * StoredAt = (const char *)DataWarehouse.StoreData((void *)s, strlen(s) + 1);
			if (StoredAt == NULL)
			{
				return NULL;
			}
			StringStorage[Used++] = StoredAt;
			IsSorted = false;
			return StoredAt;
		}
	}
}
