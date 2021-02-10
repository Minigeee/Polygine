
namespace poly
{

///////////////////////////////////////////////////////////
template <typename T>
ObjectPool Pool<T>::s_pool(sizeof(T), 32);


///////////////////////////////////////////////////////////
template <typename T>
inline T* Pool<T>::alloc()
{
	// Allocate space
	T* ptr = (T*)s_pool.alloc();
	if (!ptr) return 0;

	// Default constructor
	new(ptr)T();

	return ptr;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void Pool<T>::free(T* ptr)
{
	if (ptr)
	{
		// Call the destructor
		ptr->~T();

		// Free from pool
		s_pool.free(ptr);
	}
}

}