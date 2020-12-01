#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/OpenGL.h>
#include <poly/Graphics/RenderState.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderState RenderState::Default;


///////////////////////////////////////////////////////////
RenderState::RenderState() :
	m_colorBufferEnabled	(true),
	m_depthBufferEnabled	(true),
	m_stencilBufferEnabled	(false),
	m_colorBufferDefault	(0.0f, 0.0f, 0.0f, 1.0f),
	m_depthBufferDefault	(1.0f),
	m_stencilBufferDefault	(0),
	m_cullMode				(CullMode::Back),
	m_depthFunc				(GLComparison::Lequal)
{

}


///////////////////////////////////////////////////////////
void RenderState::apply() const
{
	// Get clear flags
	GLbitfield clearFlags = 0;

	if (m_colorBufferEnabled)
	{
		clearFlags |= GL_COLOR_BUFFER_BIT;

		// Set clear color
		glCheck(glClearColor(m_colorBufferDefault.r, m_colorBufferDefault.g, m_colorBufferDefault.b, m_colorBufferDefault.a));
	}

	if (m_depthBufferEnabled)
	{
		clearFlags |= GL_DEPTH_BUFFER_BIT;

		// Set clear depth
		glCheck(glClearDepth((float)m_depthBufferDefault));

		// Set depth function
		glCheck(glDepthFunc((GLenum)m_depthFunc));
	}

	if (m_stencilBufferEnabled)
	{
		clearFlags |= GL_STENCIL_BUFFER_BIT;

		// Set clear stencil
		glCheck(glClearStencil(m_stencilBufferDefault));
	}

	// Enable or disable depth testing
	if (m_depthBufferEnabled)
		glCheck(glEnable(GL_DEPTH_TEST));
	else
		glCheck(glDisable(GL_DEPTH_TEST));

	// Enable or disable stencil testing
	if (m_stencilBufferEnabled)
		glCheck(glEnable(GL_STENCIL_TEST));
	else
		glCheck(glDisable(GL_STENCIL_TEST));

	// Do the actual clear
	glCheck(glClear(clearFlags));

	// Set cull mode
	if (m_cullMode == CullMode::Disabled)
		glCheck(glDisable(GL_CULL_FACE));
	else
	{
		// Enable cull face
		glCheck(glEnable(GL_CULL_FACE));

		if (m_cullMode == CullMode::Front)
			glCheck(glCullFace(GL_FRONT));
		else
			glCheck(glCullFace(GL_BACK));
	}

	// Enable clip planes
	if (m_clipPlanes.size())
	{
		for (Uint32 i = 0; i < m_clipPlanes.size(); ++i)
			glCheck(glEnable(GL_CLIP_DISTANCE0 + i));
	}
	else
	{
		for (Uint32 i = 0; i < 8; ++i)
			glCheck(glDisable(GL_CLIP_DISTANCE0 + i));
	}
}


}