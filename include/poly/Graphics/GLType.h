#ifndef POLY_GL_TYPE_H
#define POLY_GL_TYPE_H

#include <poly/Core/DataTypes.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief An enum describing OpenGL data types
///
///////////////////////////////////////////////////////////
enum class GLType
{
	Unknown		= 0,
	Int8		= 0x1400,
	Uint8		= 0x1401,
	Int16		= 0x1402,
	Uint16		= 0x1403,
	Int32		= 0x1404,
	Uint32		= 0x1405,
	HalfFloat	= 0x140B,
	Float		= 0x1406,
	Double		= 0x140A
};


///////////////////////////////////////////////////////////
/// \brief An enum containing the types of alpha blending modes
///
///////////////////////////////////////////////////////////
enum class BlendFactor
{
	Zero				= 0,
	One					= 1,
	SrcColor			= 0x0300,
	OneMinusSrcColor	= 0x0301,
	DstColor			= 0x0306,
	OneMinusDstColor	= 0x0307,
	SrcAlpha			= 0x0302,
	OneMinusSrcAlpha	= 0x0303,
	DstAlpha			= 0x0304,
	OneMinusDstAlpha	= 0x0305,
	ConstColor			= 0x8001,
	OneMinusConstColor	= 0x8002
};


///////////////////////////////////////////////////////////
/// \brief Get a GLType enum from a templated variable type
///
/// \return A GLType enum
///
///////////////////////////////////////////////////////////
template <typename T>
inline GLType getGLType() { return GLType::Unknown; }

template <>
inline GLType getGLType<Int8>() { return GLType::Int8; }

template <>
inline GLType getGLType<Uint8>() { return GLType::Uint8; }

template <>
inline GLType getGLType<Int16>() { return GLType::Int16; }

template <>
inline GLType getGLType<Uint16>() { return GLType::Uint16; }

template <>
inline GLType getGLType<Int32>() { return GLType::Int32; }

template <>
inline GLType getGLType<Uint32>() { return GLType::Uint32; }

template <>
inline GLType getGLType<float>() { return GLType::Float; }

template <>
inline GLType getGLType<double>() { return GLType::Double; }


}

#endif