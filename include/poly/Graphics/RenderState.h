#ifndef POLY_RENDER_STATE_H
#define POLY_RENDER_STATE_H

#include <poly/Math/Plane.h>
#include <poly/Math/Vector4.h>

#include <vector>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief An enum defining how to cull surfaces
///
///////////////////////////////////////////////////////////
enum class CullMode
{
	Disabled,	//!< Culling faces is disabled
	Front,		//!< Cull the front face (facing camera)
	Back		//!< Cull the back face (facing away from camera)
};

///////////////////////////////////////////////////////////
/// \brief An enum defining the OpenGL comparison operators
///
///////////////////////////////////////////////////////////
enum class GLComparison
{
	Less		= 0x0201,
	Lequal		= 0x0203,
	Greater		= 0x0204,
	Gequal		= 0x0206,
	Equal		= 0x0202,
	NotEqual	= 0x0205,
	Always		= 0x0207,
	Never		= 0x0200
};

///////////////////////////////////////////////////////////
/// \brief A class containing data on OpenGL state
///
///////////////////////////////////////////////////////////
class RenderState
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	RenderState();

	///////////////////////////////////////////////////////////
	/// \brief Apply the current render state
	///
	///////////////////////////////////////////////////////////
	void apply() const;

public:
	static RenderState Default;			//!< Render state created using the default constructor

	bool m_colorBufferEnabled;			//!< True if the color buffer is enabled
	bool m_depthBufferEnabled;			//!< True if the depth buffer is enabled
	bool m_stencilBufferEnabled;		//!< True if the stencil buffer is enabled
	Colorf m_colorBufferDefault;		//!< The default clear value of the color buffer
	float m_depthBufferDefault;			//!< The default clear value of the depth buffer
	int m_stencilBufferDefault;			//!< The default clear value of the stencil buffer

	CullMode m_cullMode;				//!< The cull face mode
	GLComparison m_depthFunc;			//!< The depth function

	std::vector<Plane> m_clipPlanes;	//!< A list of clip planes
};

}

#endif