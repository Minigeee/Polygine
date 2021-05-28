#include <poly/Core/Allocate.h>
#include <poly/Core/Logger.h>

namespace poly
{

///////////////////////////////////////////////////////////
template <typename T>
bool Pool<T>::s_isInitialized = false;

///////////////////////////////////////////////////////////
template <typename T>
typename Pool<T>::PoolWrapper Pool<T>::s_wrapper;


///////////////////////////////////////////////////////////
template <typename T>
inline TypePool<T>::TypePool() :
	 m_pool			(sizeof(T))
{

}


///////////////////////////////////////////////////////////
template <typename T>
inline TypePool<T>::TypePool(Uint32 pageSize) :
	 m_pool			(sizeof(T), pageSize)
{

}


///////////////////////////////////////////////////////////
template <typename T>
inline TypePool<T>::~TypePool()
{
	// Invoke all destructors then free memory
	reset();
}


///////////////////////////////////////////////////////////
template <typename T>
inline void TypePool<T>::setPageSize(Uint32 size)
{
	m_pool.setSize(size);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 TypePool<T>::getPageSize() const
{
	return m_pool.getPageSize();
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 TypePool<T>::getNumObjects() const
{
	return m_pool.getNumObjects();
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 TypePool<T>::getNumPages() const
{
	return m_pool.getNumPages();
}


///////////////////////////////////////////////////////////
template <typename T>
inline T* TypePool<T>::alloc()
{
	// Allocate spce
	T* obj = (T*)m_pool.alloc();
	if (!obj) return 0;

	// Default constructor
	new(obj)T();

	return obj;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void TypePool<T>::free(T* ptr)
{
	// Make sure there are existing pages first
	if (!m_pool.m_firstPage || !ptr) return;

	ObjectPool::PageHeader* header = (ObjectPool::PageHeader*)m_pool.m_firstPage;
	T* page = (T*)(header + 1);

	// Find the page that contains the pointer
	while (header && (ptr < page || ptr > page + m_pool.m_pageSize * m_pool.m_objectSize))
	{
		header = (ObjectPool::PageHeader*)header->m_nextPage;
		page = (T*)(header + 1);
	}

	// If couldn't find the page that contains the pointer, quit
	if (!header)
	{
		LOG_WARNING("Tried to free memory that doesn't belong to the object pool");
		return;
	}

	// Invoke destructor
	ptr->~T();

	// Update free list and number of objects
	*(void**)ptr = header->m_nextFree;
	header->m_nextFree = ptr;
	--header->m_numObjects;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void TypePool<T>::reset()
{
	ObjectPool::PageHeader* page = (ObjectPool::PageHeader*)m_pool.m_firstPage;

	// Just free every page
	while (page)
	{
		ObjectPool::PageHeader* nextPage = (ObjectPool::PageHeader*)page->m_nextPage;

		// Keep track of which objects are free
		std::vector<bool> isFree(m_pool.m_pageSize, false);
		T* pageStart = (T*)(page + 1);

		// Follow the free list until every free slot is visited
		T* current = (T*)page->m_nextFree;
		while (current)
		{
			// Mark as free
			isFree[current - pageStart] = true;

			// Go to next free
			current = *(T**)current;
		}

		// Loop through the free list and invoke destructor for any that are filled
		for (Uint32 i = 0; i < isFree.size(); ++i)
		{
			if (!isFree[i])
				pageStart[i].~T();
		}

		// Now free the page
		ALIGNED_FREE_DBG(page);

		page = nextPage;
	}

	// Reset first page pointer
	m_pool.m_firstPage = 0;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Pool<T>::PoolWrapper::PoolWrapper() :
	m_pool		(32)
{
	s_isInitialized = true;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Pool<T>::PoolWrapper::~PoolWrapper()
{
	s_isInitialized = false;
}


///////////////////////////////////////////////////////////
template <typename T>
inline T* Pool<T>::alloc()
{
	// Allocate the object
	return s_isInitialized ? s_wrapper.m_pool.alloc() : 0;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void Pool<T>::free(T* ptr)
{
	// Free from pool
	if (s_isInitialized)
		s_wrapper.m_pool.free(ptr);
}

}