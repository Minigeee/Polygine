#include <poly/Core/ObjectPool.h>

#include <stdlib.h>
#include <cstring>

namespace poly
{


///////////////////////////////////////////////////////////
ObjectPool::ObjectPool() :
	m_firstPage		(0),
	m_objectSize	(0),
	m_pageSize		(512)
{ }


///////////////////////////////////////////////////////////
ObjectPool::ObjectPool(Uint32 objectSize, Uint32 pageSize) :
	m_firstPage		(0),
	m_objectSize	(objectSize),
	m_pageSize		(pageSize)
{ }


///////////////////////////////////////////////////////////
ObjectPool::~ObjectPool()
{
	// Free all memory
	reset();
}


///////////////////////////////////////////////////////////
ObjectPool::ObjectPool(ObjectPool&& other) :
	m_firstPage		(other.m_firstPage),
	m_objectSize	(other.m_objectSize),
	m_pageSize		(other.m_pageSize)
{
	other.m_firstPage = 0;
	other.m_objectSize = 0;
	other.m_pageSize = 0;
}


///////////////////////////////////////////////////////////
ObjectPool& ObjectPool::operator=(ObjectPool&& other)
{
	if (&other != this)
	{
		m_firstPage = m_firstPage;
		m_objectSize = m_objectSize;
		m_pageSize = m_pageSize;

		other.m_firstPage = 0;
		other.m_objectSize = 0;
		other.m_pageSize = 0;
	}

	return *this;
}


///////////////////////////////////////////////////////////
void ObjectPool::setObjectSize(Uint32 size)
{
	ASSERT(size >= sizeof(void*), "The size of the object must be at least the size of a pointer");

	if (!m_firstPage)
		m_objectSize = size;
}


///////////////////////////////////////////////////////////
void ObjectPool::setPageSize(Uint32 size)
{
	ASSERT(size >= 1, "The size of a page must be at least 1");

	if (!m_firstPage)
		m_pageSize = size;
}


///////////////////////////////////////////////////////////
Uint32 ObjectPool::getObjectSize() const
{
	return m_objectSize;
}


///////////////////////////////////////////////////////////
Uint32 ObjectPool::getPageSize() const
{
	return m_pageSize;
}


///////////////////////////////////////////////////////////
Uint32 ObjectPool::getNumObjects() const
{
	Uint32 numObjects = 0;

	// Iterate through pages and add up objects
	PageHeader* header = (PageHeader*)m_firstPage;
	while (header)
	{
		numObjects += header->m_numObjects;
		header = (PageHeader*)header->m_nextPage;
	}

	return numObjects;
}


///////////////////////////////////////////////////////////
Uint32 ObjectPool::getNumPages() const
{
	Uint32 numPages = 0;

	// Iterate through pages and add up objects
	PageHeader* header = (PageHeader*)m_firstPage;
	while (header)
	{
		++numPages;
		header = (PageHeader*)header->m_nextPage;
	}

	return numPages;
}


///////////////////////////////////////////////////////////
void* ObjectPool::alloc()
{
	// First check if any pages have been allocated yet
	if (!m_firstPage)
		m_firstPage = allocPage();

	PageHeader* header = (PageHeader*)m_firstPage;

	// Find first page that has slots available
	while (header->m_numObjects == m_pageSize)
	{
		// Break early if the current page is full and there is no next page
		if (!header->m_nextPage)
			break;

		header = (PageHeader*)header->m_nextPage;
	}

	// If couldn't find a non-full page, create a new page
	if (header->m_numObjects == m_pageSize && !header->m_nextPage)
	{
		// Link current page to next page
		header->m_nextPage = allocPage();
		header = (PageHeader*)header->m_nextPage;
	}

	// Reserve spot for the object
	void* obj = header->m_nextFree;

	// Update next free slot and number of objects
	header->m_nextFree = *(void**)header->m_nextFree;
	++header->m_numObjects;

#ifndef NDEBUG
	// In debug mode, keep track of which slots are being used to prevent double freeing
	Uint32 index = ((Uint8*)obj - (Uint8*)(header + 1)) / m_objectSize;
	header->m_used[index] = true;
#endif

	// Set requested memory to 0 and return
	memset(obj, 0, m_objectSize);
	return obj;
}


///////////////////////////////////////////////////////////
void ObjectPool::free(void* ptr)
{
	// Make sure there are existing pages first
	if (!m_firstPage || !ptr) return;

	PageHeader* header = (PageHeader*)m_firstPage;
	Uint8* page = (Uint8*)(header + 1);

	// Find the page that contains the pointer
	while (header && (ptr < page || ptr > page + m_pageSize * m_objectSize))
	{
		header = (PageHeader*)header->m_nextPage;
		page = (Uint8*)(header + 1);
	}

	// Error if the object pool doesn't contain the pointer (this shouldn't be allowed to happen)
	ASSERT(header, "Tried to free memory that doesn't belong to the object pool");

#ifndef NDEBUG
	// In debug mode, keep track of which slots are being used to prevent double freeing
	Uint32 index = ((Uint8*)ptr - (Uint8*)(header + 1)) / m_objectSize;
	ASSERT(header->m_used[index], "The pointer 0x%08X is being freed from the object pool more than once, this will cause undefined behavior in release builds", (int)ptr);

	header->m_used[index] = false;
#endif

	// Update free list and number of objects
	*(void**)ptr = header->m_nextFree;
	header->m_nextFree = ptr;
	--header->m_numObjects;
}


///////////////////////////////////////////////////////////
void ObjectPool::reset()
{
	PageHeader* page = (PageHeader*)m_firstPage;

	// Just free every page
	while (page)
	{
		PageHeader* nextPage = (PageHeader*)page->m_nextPage;

#ifndef NDEBUG
		page->~PageHeader();
#endif
		ALIGNED_FREE_DBG(page);

		page = nextPage;
	}

	// Reset first page pointer
	m_firstPage = 0;
}


///////////////////////////////////////////////////////////
void* ObjectPool::allocPage()
{
	// Calculate size of page
	Uint32 pageSize = m_pageSize * m_objectSize + sizeof(PageHeader);

	// Allocate aligned memory
	PageHeader* header = (PageHeader*)ALIGNED_MALLOC_DBG(pageSize, alignof(std::max_align_t));

	// Get the start location of the page
	Uint8* page = (Uint8*)(header + 1);

	// Initialize metadata
#ifndef NDEBUG
	new(header)PageHeader();
	header->m_used.resize(m_pageSize);
#endif
	header->m_nextPage = 0;
	header->m_nextFree = page;
	header->m_numObjects = 0;

	// Setup free list
	for (Uint32 i = 0; i < m_pageSize - 1; ++i)
		*(void**)(page + i * m_objectSize) = page + (i + 1) * m_objectSize;
	*(void**)(page + (m_pageSize - 1) * m_objectSize) = 0;

	// Return the page (which starts at the header)
	return header;
}


}