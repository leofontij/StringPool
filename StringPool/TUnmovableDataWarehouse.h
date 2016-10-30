#pragma once
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

