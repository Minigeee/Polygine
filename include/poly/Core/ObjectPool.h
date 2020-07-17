#ifndef POLY_OBJECT_POOL_H
#define POLY_OBJECT_POOL_H

#include <poly/Core/DataTypes.h>

#include <vector>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A data container for objects of the same size
///
///////////////////////////////////////////////////////////
class ObjectPool
{
public:

    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty object pool that has an initial object
    /// size of 0
    ///
    /// \see setObjectSize
    /// 
    ///////////////////////////////////////////////////////////
    ObjectPool();
    ObjectPool(Uint32 objectSize, Uint32 pageSize = 512);

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    ObjectPool(ObjectPool&& other);
    ObjectPool& operator=(ObjectPool&& other);

    ///////////////////////////////////////////////////////////
    /// \brief Cleans up memory used by the object pool
    ///
    ///////////////////////////////////////////////////////////
    ~ObjectPool();

    void setObjectSize(Uint32 size);
    void setPageSize(Uint32 size);
    Uint32 getObjectSize() const;
    Uint32 getPageSize() const;
    Uint32 getNumObjects() const;
    Uint32 getNumPages() const;

    void* alloc();
    void free(void* ptr);
    void reset();

private:
    struct PageHeader
    {
        void* m_nextPage;
        void* m_nextFree;
        Uint32 m_numObjects;
    };

private:
    void* allocPage();

private:
    void* m_firstPage;
    Uint32 m_objectSize;
    Uint32 m_pageSize;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::ObjectPool
/// \ingroup Core
///
/// The object pool is a data container that can hold
/// objects of the same size. When allocating a new object,
/// the object pool will no longer shift the object around
/// in memory. This means that a pointer to the allocated
/// object will be valid as long as the object pool still
/// exists and the object hasn't been freed from memory.
/// 
///////////////////////////////////////////////////////////