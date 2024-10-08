#ifndef POLY_ALLOCATE_H
#define POLY_ALLOCATE_H

#include <poly/Core/DataTypes.h>

#include <string>
#include <vector>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
/// \brief A utility class used by the macro functions
///
/// This class is a utility class used by macro allocation
/// and free functions, for the purpose of tracking memory
/// allocations and frees in debug mode to track memory leaks.
///
/// Make sure to include <poly/Core/Allocate.h> first to ensure
/// that all memory allocations will be caught.
///
///////////////////////////////////////////////////////////
class Allocate
{
public:
	Allocate();

	~Allocate();

	void* alloc(Uint32 size, const std::string& file, Uint32 line);

	void free(void* ptr);

	void* alignedAlloc(Uint32 size, Uint32 align, const std::string& file, Uint32 line);

	void alignedFree(void* ptr);

private:
	struct AllocData
	{
		std::string m_info;
		void* m_address;
		Uint32 m_size;
	};

private:
	HashMap<void*, AllocData> m_data;
	Uint32 m_test;
};


#ifndef DOXYGEN_SKIP

///////////////////////////////////////////////////////////
extern Allocate* g_allocate;


///////////////////////////////////////////////////////////
class AllocateInit
{
public:
	AllocateInit();

	~AllocateInit();

private:
	static Uint32 s_refCount;
};


///////////////////////////////////////////////////////////
static AllocateInit g_allocateInit;

#endif


}


#ifdef NDEBUG
#define MALLOC_DBG(size) poly::priv::g_allocate->alloc(size, __FILE__, __LINE__)
#define FREE_DBG(ptr) poly::priv::g_allocate->free(ptr)
#define ALIGNED_MALLOC_DBG(size, align) poly::priv::g_allocate->alignedAlloc(size, align, __FILE__, __LINE__)
#define ALIGNED_FREE_DBG(ptr) poly::priv::g_allocate->alignedFree(ptr)
#else
#define MALLOC_DBG(size) ::malloc(size)
#define FREE_DBG(ptr) ::free(ptr)
#ifdef WIN32
#define ALIGNED_MALLOC_DBG(size, align) ::_aligned_malloc(size, align)
#define ALIGNED_FREE_DBG(ptr) ::_aligned_free(ptr)
#else
#define ALIGNED_MALLOC_DBG(size, align) ::aligned_malloc(size, align)
#define ALIGNED_FREE_DBG(ptr) ::aligned_free(ptr)
#endif
#endif

}

#endif