#pragma once
namespace lefoso
{
	namespace system
	{
		/**********************************************************************************************//**
		 * \class	TUnmovableDataWarehouse
		 *
		 * \brief	An unmovable data warehouse.
		 * 			
		 * 			This class stores chunks of data. It has no knowledge about the data. The warehouse
		 * 			garantees two things:
		 * 				- Data will not be deleted (unless the warehouse is destroyed)  
		 * 				- Data will always at the same location.  
		 * 			The warehouse is divided in departments. Basically departments can have two sizes.
		 * 			The first department will have a specified initial size. When the warehouse is
		 * 			running out of space a new department will be opened with a given incremental size.
		 * 			An exception is made when a block of data is to be stored that is greater that the
		 * 			given department size. In that case a private department is opened for that block.\n
		 * 			It is unlikely that a department will be completely filled. To prevent time consuming
		 * 			searching for space in the departments a limit of wasted space can be specified for
		 * 			the departments. If the free space in a department get belows this limit the
		 * 			department is considered to be full.\n To prevent to run out of memory an overall
		 * 			maximum storage space can be specified.  
		 *
		 * \author	Leo
		 * \date	2016-11-26
		 **************************************************************************************************/

		class TUnmovableDataWarehouse
		{
		public:
			TUnmovableDataWarehouse(size_t initialSize, size_t incrementSize, size_t maxSize, size_t maxWasted, size_t alignment);
			~TUnmovableDataWarehouse();
			void * StoreData(void * data, size_t dataSize);
			static size_t RoundUp(size_t size, size_t alignment);
		private:
			size_t InitialSize;			//!< Size that is allocate when object is constructed
			size_t IncrementSize;		//!< Size that is added when object runs out of storage space
			size_t MaxSize;				//!< Maximum size that is allocate in total
			size_t MaxWasted;			//!< Maximum that may be wasted in each department 
			size_t Alignment;			//!< Alignment (will typically be sizeof(int))
			int NumberOfDepartments;	//!< Number of departments in the warehouse

			class TWarehouseDepartment
			{
			public:
				TWarehouseDepartment(size_t capacity, size_t maxWasted, size_t alignment);
				~TWarehouseDepartment();
				void * StoreData(void * data, size_t dataSize);
				bool IsFull();
			private:
				size_t Capacity;	//!< Capacity of storage space(rounded up to next multiple of Alignment)
				size_t MaxWasted;	//!< Maximum of storage capacity that may be wasted
				size_t Alignment;	//!< Alignment (will typically be sizeof(int))
				size_t Used;		//!< Amount of storage space in use.
				char * StorageArea; //!< Allocated storage area
			};

			TWarehouseDepartment ** WarehouseDepartments;	//!< Array containing pointers to warehouse departments
			int NumberOfWarehouseDepartments;				//!< Number of warehouse departments
			int FirstNonFullDepartPartment;					//!< Index of first non-full department
			size_t	TotalCapacity;							//!< Total capacity of the warehouse
			size_t  TotalUsed;								//!< Total amount used

		};
	} // end of system namespace
} // end of lefoso namespace
