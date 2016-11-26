#pragma once
#include "TUnmovableDataWarehouse.h"
namespace lefoso
{
	namespace system
	{
		class TConstStringPool
		{
		public:
			TConstStringPool(size_t initialStrings, size_t incrementStrings, size_t maxStrings,
				size_t initialStorage, size_t incrementStorage, size_t maxStorage);
			~TConstStringPool();
			const char * AddString(const char * s);
		private:
			const char * FindString(const char * s);
			const char * AddNewString(const char * s);
			void Sort();
			static int __cdecl fsort(const void * p1, const void * p2);
			TUnmovableDataWarehouse DataWarehouse;	//!< Warehouse where string data is stored 
			const char **StringStorage;				//!< Allocated array in which the pointers to string are stored
			size_t Capacity;			//!< Capacity expressed in number of strings
			size_t Used;				//!< Used storage expressed in strings
			size_t MaxStrings;			//!< Overall maximum number of string
			size_t InitialStrings;		//!< Initial capacity expressed in number of strings
			size_t IncrementStrings;	//!< Capacity increment expressed in number of strings
			bool IsSorted;				//!< Flag indicating that strings are in sorted order
		};
	}
}