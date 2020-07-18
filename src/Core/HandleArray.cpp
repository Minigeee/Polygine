#include <poly/Core/HandleArray.h>

using namespace poly;

///////////////////////////////////////////////////////////

Handle::Handle() :
	m_index		(0),
	m_counter	(0)
{ }

Handle::Handle(Uint16 index, Uint16 counter) :
	m_index		(index),
	m_counter	(counter)
{ }

Handle::operator Uint32() const
{
	return (((Uint32)m_index) << 16) | m_counter;
}

///////////////////////////////////////////////////////////