#ifndef POLY_HANDLE_ARRAY_H
#define POLY_HANDLE_ARRAY_H

#include <poly/Core/DataTypes.h>

#include <vector>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief The structure used to access elements in the handle array
///
///////////////////////////////////////////////////////////
struct Handle
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Handle();

	///////////////////////////////////////////////////////////
	/// \brief Construct a handle with an index and a counter
	///
	///////////////////////////////////////////////////////////
	Handle(Uint16 index, Uint16 counter = 0);

	operator Uint32() const;

	Uint16 m_index;		//!< Index of handle, used to access correct element
	Uint16 m_counter;	//!< Counter used to ensure the handled element hasn't been removed
};


///////////////////////////////////////////////////////////
/// \brief An array that is accessed by handles instead of by index
///
///////////////////////////////////////////////////////////
template <typename T>
class HandleArray
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Initializes all internal arrays with no size
	///
	///////////////////////////////////////////////////////////
	HandleArray();

	///////////////////////////////////////////////////////////
	/// \brief Construct with a certain amount of reserved space
	///
	/// \param size Amount of objects to reserve space for
	///
	///////////////////////////////////////////////////////////
	HandleArray(Uint16 size);

	///////////////////////////////////////////////////////////
	/// \brief Access the array by handle
	///
	/// Access the array, similar to std::vector, but by handle.
	/// The will throw an exception if an invalid handle is used.
	/// If the element that is being referenced by the handle
	/// has been removed, the accessor will fail and throw an exception.
	///
	/// \return Referenced to the element referenced by the handle
	///
	///////////////////////////////////////////////////////////
	T& operator[](Handle handle);

	///////////////////////////////////////////////////////////
	/// \brief Add an element to the array and get its handle
	///
	/// Elements that are added to the array will be kept in a
	/// internal contiguous array and are accessed by the returned
	/// handle. The handle returned will always be valid until the
	/// element being referenced by the handle is removed.
	///
	/// \param element The element to add
	///
	/// \return Handle used to access the added element
	///
	/// \see remove
	///
	///////////////////////////////////////////////////////////
	Handle add(const T& element);

	///////////////////////////////////////////////////////////
	/// \brief Remove the element being referenced by the handle
	///
	/// When removing an element, the internal array used to store
	/// the element keeps all its data contiguous. The element is
	/// removed using swap-pop removal, so removal requires a single
	/// swap no matter how large the array may be. Even when the
	/// elements shift around in memory, the element's handle will
	/// always point to the correct element.
	///
	/// When an element is removed, any following attempts to
	/// access the element using its handle will fail.
	///
	/// \param handle Handle of the element to remove
	///
	/// \see add
	///
	///////////////////////////////////////////////////////////
	void remove(Handle handle);

	///////////////////////////////////////////////////////////
	/// \brief Completely reset the handle array
	///
	/// Resets the array to its state after the default constructor.
	/// This will invoke the destructors of any element that was
	/// being stored in the array.
	///
	///////////////////////////////////////////////////////////
	void reset();

	///////////////////////////////////////////////////////////
	/// \brief Get the number of elements in the array
	///
	/// \return Number of elements
	///
	///////////////////////////////////////////////////////////
	Uint16 size() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the amount of reserved memory in number of elements
	///
	/// \return Amount of reserved space
	///
	///////////////////////////////////////////////////////////
	Uint16 capacity() const;

	///////////////////////////////////////////////////////////
	/// \brief See if the array is empty
	///
	/// \return True if the array is empty
	///
	///////////////////////////////////////////////////////////
	bool isEmpty() const;


	///////////////////////////////////////////////////////////
	/// \brief Get the contiguous internal array
	///
	/// Data in this array is always kept contiguous. However,
	/// its contents are not guarenteed to be in the same order
	/// they were added to the array.
	///
	/// \return The internal array used to store the elements
	///
	///////////////////////////////////////////////////////////
	std::vector<T>& getData();

private:
	std::vector<T> m_data;				//!< Internal data array
	std::vector<Handle> m_handleToData;	//!< Maps handle index to actual index, also keeps a counter to detect invalid handles
	std::vector<Uint16> m_dataToHandle; //!< Maps actual index to handle index
	Uint16 m_nextFree;					//!< Index of the next free handle
};

#include <poly/Core/HandleArray.inl>

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::HandleArray
/// \ingroup Core
///
/// A data container that accesses elements by handles instead
/// of by index. The elements that are added to the array will
/// be kept in a internal contiguous array, and all elements
/// in this internal array will be kept contiguous, even if some
/// elements are removed. The main purpose of the handle array
/// is to have a way to consistently access elements in an
/// array that keeps its elements in contiguous storage. In
/// a normal array, if an element is removed, then all other
/// elements that came after the one that was removed has to be
/// shifted to fill in the empty slot. The problem with shifting
/// the elements is that the indices that were used to access them
/// before will become invalid.
///
/// For example:
/// \code
/// std::vector<int> v;
/// v.push_back(3);
/// v.push_back(1);
/// v.push_back(4);
/// v.push_back(1);
/// v.push_back(5);
///
/// // For whatever reason, we would like to keep a reference to
/// // the element "4", so we store the index it is at
/// int index = 2;
///
/// std::cout << v[index] << "\n"; // Prints "4"
/// 
/// // If we remove an element that comes before, the index that
/// // we stored will become invalid
/// v.erase(v.begin());
///
/// // The index that we stored is now invalid, it no longer points
/// // to the correct element
/// std::cout << v[index] << "\n"; // Prints "1"
///
/// \endcode
///
/// This is a very basic example, and we would have no use for storing
/// the index, but it serves as an example.
/// By using a handle array, elements could be removed and they could
/// kept contiguous in memory, but the handles used to access them
/// would still be valid.
///
/// \code
/// using namespace poly;
///
/// HandleArray<int> a;
/// Handle h1 = a.add(3);
/// Handle h2 = a.add(1);
/// Handle h3 = a.add(4);
/// Handle h4 = a.add(1);
/// Handle h5 = a.add(5);
///
/// std::cout << a[h3] << "\n"; // Prints "4"
///
/// // Remove the first element
/// a.remove(h1);
///
/// std::cout << a[h3] << "\n"; // Still prints "4"
///
/// int removedValue = a[h1]; // This would throw an exception because h1 was removed
///
/// \endcode
/// 
///////////////////////////////////////////////////////////