#ifndef POLY_TYPE_INFO_H
#define POLY_TYPE_INFO_H

#include <poly/Core/DataTypes.h>

#include <string>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief Struct that contains information on variable types
///
///////////////////////////////////////////////////////////
struct TypeInfo
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Get the unique numerical ID of the variable type
	///
	/// \return The numerical ID of the type specified in the template parameter
	///
	///////////////////////////////////////////////////////////
	template <typename T> static Uint32 getId();

	///////////////////////////////////////////////////////////
	/// \brief Get the type info struct of the specified type
	///
	/// \return A type info struct
	///
	/// \see addInfo
	///
	///////////////////////////////////////////////////////////
	template <typename T> static const TypeInfo& get();

	///////////////////////////////////////////////////////////
	/// \brief Get the type info struct of the specified type ID
	///
	/// \param typeId The ID of the variable type to retrieve
	///
	/// \return A type info struct
	///
	/// \see addInfo
	///
	///////////////////////////////////////////////////////////
	static const TypeInfo& get(Uint32 typeId);

	Uint32 m_id;			//!< The numerical ID of the variable type
	Uint32 m_size;			//!< The size of the type in bytes
	Uint32 m_align;			//!< The alignment of the type in bytes
	Uint32 m_hash;			//!< The hash value of the type's name (consistent)
	std::string m_name;		//!< The name of the variable type

	bool m_isPod;			//!< True if the type is POD (Plain old data)
	bool m_isLiteral;		//!< True if the type is a literal type
	bool m_isAbstract;		//!< True if the type is an abstract class
	bool m_isPolymorphic;	//!< True if the type is a polymorphic class

private:
	static Uint32 typeCounter;						//!< Used to assign type IDs
	static HashMap<Uint32, TypeInfo> idToInfo;		//!< Maps type ID to type info struct
};

}

#include <poly/Core/TypeInfo.inl>

#endif

///////////////////////////////////////////////////////////
/// \struct poly::TypeInfo
/// \ingroup Core
///
/// poly::TypeInfo is a helper class that can retrieve and
/// store information on type data. This is the infomation
/// that the struct stores:
///
/// \li Type ID
/// \li Name
/// \li Size (in bytes)
/// \li Alignment (in bytes)
/// \li Is it a POD type?
/// \li Is it a literal?
/// \li Is it abstract?
/// \li Is it polymorphic?
///
/// The name of the type, however, has to be provided by the
/// user when calling TypeInfo::init<T>("T"). To get the type
/// info for a specific type, the static function init<T>() must
/// be called, where the template parameter T is the type
/// you wish to retrieve type info for. Afterwards,
/// using the static function getInfo<T>() will return a TypeInfo
/// struct containing all the correct data.
///
/// Usage example:
/// \code
/// using namespace poly;
///
/// // Create a few classes
/// class A { };
/// class B { virtual void test() = 0; };
/// class C : public B { };
///
/// TypeInfo::init<int>("int");
/// TypeInfo::init<float>("float");
/// TypeInfo::init<A>("A");
/// TypeInfo::init<B>("B");
/// TypeInfo::init<C>("C");
///
/// TypeInfo::getInfo<int>().m_name;		// "int"
/// TypeInfo::getInfo<int>().m_size;		// 4
/// TypeInfo::getInfo<float>().m_align;		// 4
/// TypeInfo::getInfo<int>().m_isPod;		// true
/// TypeInfo::getInfo<A>().m_isPod;			// true
/// TypeInfo::getInfo<B>().m_isPod;			// false
/// TypeInfo::getInfo<B>().m_isAbstract;	// true
/// TypeInfo::getInfo<C>().m_isAbstract;	// true
/// TypeInfo::getInfo<C>().m_isPolymorphic; // true
///
/// \endcode
///
/// Also note that pointers, references, const references, etc...
/// count as seperate types.
///
///////////////////////////////////////////////////////////