#ifndef POLY_OBJECT_POOL_H
#define POLY_OBJECT_POOL_H

#include <poly/Core/DataTypes.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A memory allocator for allocating objects of the same size
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

    ///////////////////////////////////////////////////////////
    /// \brief Construct and set the object size and page size
    ///
    /// \param objectSize Size of each object slot in bytes
    /// \param pageSize Size of each page in number of objects
    ///
    /// \see setObjectSize
    /// \see setPageSize
    ///
    ///////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////
    /// \brief Set the size of each object slot in the object pool
    ///
    /// Each time an allocation is requested, the object pool will
    /// allocate a space of the specified size. Object size has
    /// to be a number greater than or equal to 4 (default 0) to
    /// be valid. All allocations will fail otherwise.
    ///
    /// \param size Size of the object slot in bytes
    ///
    ///////////////////////////////////////////////////////////
    void setObjectSize(Uint32 size);

    ///////////////////////////////////////////////////////////
    /// \brief Set the size of each page in number of objects
    ///
    /// A page is just a static collection of object slots. A page
    /// acts similar to an array, where there is a fixed number of
    /// elements after creation and each element is the same size.
    /// The difference is that there are allowed to be gaps, where
    /// the slots that are actively being used don't have to be
    /// next to each other. If the current page of the object
    /// pool is filled up, the object pool will allocate a new
    /// page of the same size next time an allocation is requested.
    ///
    /// For example, if the page size is set to 100 (default 512),
    /// and there have been 99 allocations, the next allocation
    /// will take the last available slot in the current page.
    /// The next allocation after that won't find any space in
    /// the current page, so the object pool will create a new
    /// page and take space for the allocation from the new page
    /// instead.
    ///
    /// Page size has to be greater than or equal to 1 for
    /// any allocation requests to be valid.
    ///
    /// \param size The size of each page in number of objects
    ///
    ///////////////////////////////////////////////////////////
    void setPageSize(Uint32 size);

    ///////////////////////////////////////////////////////////
    /// \brief Get the object size
    ///
    /// \return The size of each object slot in bytes
    ///
    /// \see setObjectSize
    ///
    ///////////////////////////////////////////////////////////
    Uint32 getObjectSize() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the page size
    ///
    /// \return The size of each page in number of objects
    ///
    /// \see setPageSize
    ///
    ///////////////////////////////////////////////////////////
    Uint32 getPageSize() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of object allocations that have been made
    ///
    /// Every time an allocation is made, the number of objects
    /// will increase by one. This number will decrease every
    /// time an allocated object is freed.
    ///
    /// \return The number of objects in the pool
    ///
    ///////////////////////////////////////////////////////////
    Uint32 getNumObjects() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of pages in the object pool
    ///
    /// Every time a page is filled up, a new page is created
    /// to store allocations in. Pages will continue to exist
    /// after they are created, unless the object pool is reset.
    ///
    /// \return The number of pages that exist
    ///
    ///////////////////////////////////////////////////////////
    Uint32 getNumPages() const;

    ///////////////////////////////////////////////////////////
    /// \brief Allocate a new slot in the object pool
    ///
    /// The amount of space allocated will be equal to the
    /// object size that was set. This function will sometimes
    /// faile and return NULL if invalid parameters are set.
    /// If the object size is less than 4 or if the page size is
    /// less than 1, allocation will fail.
    ///
    /// \return A pointer to the allocated space
    ///
    ///////////////////////////////////////////////////////////
    void* alloc();

    ///////////////////////////////////////////////////////////
    /// \brief Free memory that was previously allocated by the pool
    ///
    /// If the pointer is NULL or if the pointer does not belong
    /// to the pool, nothing will happen to the pool and no exceptions
    /// will be thrown.
    ///
    ///////////////////////////////////////////////////////////
    void free(void* ptr);

    ///////////////////////////////////////////////////////////
    /// \brief Reset the object pool
    ///
    /// Does a full reset to its state right after being constructed.
    /// This will free all the memory it is using and will reset
    /// all of its data. This will not invoke any of the destructors
    /// of the objects previously being held in the pool.
    ///
    ///////////////////////////////////////////////////////////
    void reset();

private:
    ///////////////////////////////////////////////////////////
    /// \brief Allocate a new page of memory
    ///
    ///////////////////////////////////////////////////////////
    void* allocPage();

    ///////////////////////////////////////////////////////////
    // Types
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    /// \brief Header structure that holds metadata for each page
    ///
    ///////////////////////////////////////////////////////////
    struct PageHeader
    {
        void* m_nextPage;       //!< A pointer to the next page
        void* m_nextFree;       //!< A pointer to the next free object slot
        Uint32 m_numObjects;    //!< The number of objects currently in the page
    };

    ///////////////////////////////////////////////////////////
    // Variables
    ///////////////////////////////////////////////////////////
    void* m_firstPage;      //!< Pointer to the first page of objects
    Uint32 m_objectSize;    //!< Size of each object in bytes
    Uint32 m_pageSize;      //!< Size of each page in number of objects
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::ObjectPool
/// \ingroup Core
///
/// The object pool is a memory allocator that can hold a collection
/// objects of the same size. Its purpose is to increase allocation
/// speed if many object allocations are required, and to minimize
/// memory fragmentation. The object pool should be used in
/// place of the new operator because of the reasons stated.
///
/// The object pool internally creates a space where many objects
/// of the same size can reserve, and because every slot is the
/// the same size, no fragmentation can occur. If a large number
/// of objects is required, the object pool will allocate more
/// "pages" to store these objects. A page is just a static
/// collection of object slots. They will not change size or
/// move in memory once they are created. With the use of pages,
/// object pools can grow to however big is needed.
///
/// Usage example:
/// \code
/// using namespace poly;
///
/// // Create a pool for objects that are 8 bytes, and make each page 100 objects
/// ObjectPool pool(8, 100);
///
/// // Allocate space for a double
/// double* pi = (double*)pool.alloc();
/// *pi = 3.1415;
///
/// std::cout << pool.getNumObjects() << "\n";  // 1 object
/// std::cout << pool.getNumPages() << "\n";    // 1 page
///
/// for (int i = 0; i < 100; ++i)
///     pool.alloc();
///
/// std::cout << pool.getNumObjects() << "\n";  // 101 objects
/// std::cout << pool.getNumPages() << "\n";    // 2 pages
///
/// // Free the first object
/// pool.free(pi);
///
/// // Reset the object pool
/// pool.reset();
///
/// \endcode
/// 
///////////////////////////////////////////////////////////