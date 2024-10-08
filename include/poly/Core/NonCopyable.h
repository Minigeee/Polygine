#ifndef POLY_NON_COPYABLE_H
#define POLY_NON_COPYABLE_H

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A class for non-copyable types
///
///////////////////////////////////////////////////////////
class NonCopyable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	NonCopyable() { }

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~NonCopyable() { }

private:
	///////////////////////////////////////////////////////////
	/// \brief A private copy constructor
	///
	/// Classes outside will not be able to copy non-copyables
	/// and giving it no definition will cause the compiler to
	/// throw an error.
	///
	///////////////////////////////////////////////////////////
	NonCopyable(const NonCopyable&);

	///////////////////////////////////////////////////////////
	/// \brief A private copy constructor
	///
	/// Classes outside will not be able to copy non-copyables
	/// and giving it no definition will cause the compiler to
	/// throw an error.
	///
	///////////////////////////////////////////////////////////
	NonCopyable& operator=(const NonCopyable&);
};


///////////////////////////////////////////////////////////
/// \brief A class for non-moveable types
///
///////////////////////////////////////////////////////////
class NonMoveable : public NonCopyable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	NonMoveable() { }

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~NonMoveable() { }

private:
	///////////////////////////////////////////////////////////
	/// \brief A private move constructor
	///
	/// Classes outside will not be able to move non-moveable
	/// and giving it no definition will cause the compiler to
	/// throw an error.
	///
	///////////////////////////////////////////////////////////
	NonMoveable(NonMoveable&&);

	///////////////////////////////////////////////////////////
	/// \brief A private move constructor
	///
	/// Classes outside will not be able to move non-moveable
	/// and giving it no definition will cause the compiler to
	/// throw an error.
	///
	///////////////////////////////////////////////////////////
	NonMoveable& operator=(NonMoveable&&);
};


}

#endif