///////////////////////////////////////////////////////////

template <typename T>
inline HandleArray<T>::HandleArray() :
	m_nextFree(0)
{ }

template <typename T>
inline HandleArray<T>::HandleArray(Uint16 size) :
	m_nextFree(0)
{
	m_data.reserve(size);
	m_handleToData.resize(size);
	m_dataToHandle.resize(size, 0);

	// Setup free list
	for (Uint16 i = 0; i < size; ++i)
		m_handleToData[i].m_index = i + 1;
}

///////////////////////////////////////////////////////////

template <typename T>
inline T& HandleArray<T>::operator[](Handle handle)
{
	Handle entry = m_handleToData[handle.m_index];

	// Make sure the handle is valid
	if (entry.m_counter != handle.m_counter)
		LOG_ERROR("Invalid handle: " + std::to_string(handle.m_index));

	return m_data[entry.m_index];
}

///////////////////////////////////////////////////////////

template <typename T>
inline Handle HandleArray<T>::add(const T& element)
{
	// Resize the arrays if the next free handle is out of bounds
	if (m_nextFree >= m_handleToData.size())
	{
		m_handleToData.push_back(Handle((Uint16)(m_handleToData.size() + 1)));
		m_dataToHandle.push_back(0);
	}

	// Add element to data array
	m_data.push_back(element);

	// Now generate a handle
	Handle handle(m_nextFree, m_handleToData[m_nextFree].m_counter);

	// Map handle to actual position using the lookup table
	// First, update the next free handle
	m_nextFree = m_handleToData[handle.m_index].m_index;

	// Then point to element position
	m_handleToData[handle.m_index].m_index = (Uint16)(m_data.size() - 1);
	// Point position to handle (required info for removal)
	m_dataToHandle[m_data.size() - 1] = handle.m_index;

	return handle;
}

template <typename T>
inline void HandleArray<T>::remove(Handle handle)
{
	Uint16 pos = m_handleToData[handle.m_index].m_index;

	// Swap pop
	// Not the most efficient swap, but no other way to properly call the destructor
	std::swap(m_data[pos], m_data.back());
	m_data.pop_back();

	// Get index of the handle of the element that was moved
	Uint16 movedHandleIndex = m_dataToHandle[m_data.size()];

	// Update the index the moved handle points to
	m_handleToData[movedHandleIndex].m_index = pos;

	// Update the handle the targeted index position points to
	m_dataToHandle[pos] = movedHandleIndex;

	// Update free list
	// Store the previous free handle in the one that was just freed
	m_handleToData[handle.m_index].m_index = m_nextFree;
	// Make next free point to the handle that was freed
	m_nextFree = handle.m_index;

	// Increment counter to invalidate any existing handles
	++m_handleToData[handle.m_index].m_counter;
}

template <typename T>
inline void HandleArray<T>::reset()
{
	// Just completely reset everything
	m_data = std::vector<T>();
	m_handleToData = std::vector<Handle>();
	m_dataToHandle = std::vector<Uint16>();
	m_nextFree = 0;
}

///////////////////////////////////////////////////////////

template <typename T>
inline Uint16 HandleArray<T>::size() const
{
	return (Uint16)m_data.size();
}

template <typename T>
inline Uint16 HandleArray<T>::capacity() const
{
	return (Uint16)m_data.capacity();
}

template <typename T>
inline bool HandleArray<T>::isEmpty() const
{
	return m_data.empty();
}

///////////////////////////////////////////////////////////

template <typename T>
inline std::vector<T>& HandleArray<T>::getData()
{
	return m_data;
}

///////////////////////////////////////////////////////////