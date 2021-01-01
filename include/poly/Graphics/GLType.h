#ifndef POLY_GL_TYPE_H
#define POLY_GL_TYPE_H

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief An enum describing OpenGL data types
///
///////////////////////////////////////////////////////////
enum class GLType
{
	Unknown	= 0,
	Int8	= 0x1400,
	Uint8	= 0x1401,
	Int16	= 0x1402,
	Uint16	= 0x1403,
	Int32	= 0x1404,
	Uint32	= 0x1405,
	Float	= 0x1406,
	Double	= 0x140A
};

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
	OneMinusDstAlpha	= 0x0305
};

}

#endif