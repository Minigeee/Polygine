#ifndef POLY_POST_PROCESS_H
#define POLY_POST_PROCESS_H

#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief The base class for all post processing effects
///
///////////////////////////////////////////////////////////
class PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Render a post processing effect, given an input framebuffer and an output framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	virtual void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) = 0;

private:
	static VertexArray quadVao;
	static VertexBuffer quadVbo;

protected:
	static VertexArray& getVertexArray();
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect that applies gamma correction and HDR rendering
///
///////////////////////////////////////////////////////////
class ColorAdjust : public PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	ColorAdjust();

	///////////////////////////////////////////////////////////
	/// \brief Create the effect from a gamma factor
	///
	///////////////////////////////////////////////////////////
	ColorAdjust(float gamma);

	///////////////////////////////////////////////////////////
	/// \brief Apply gamma correction and HDR rendering to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the gamma factor
	///
	/// \param gamma The gamma factor
	///
	///////////////////////////////////////////////////////////
	void setGamma(float gamma);

	///////////////////////////////////////////////////////////
	/// \brief Get the gamma factor
	///
	/// \return The gamma factor
	///
	///////////////////////////////////////////////////////////
	float getGamma() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	float m_gamma;		//!< The gamma factor
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::PostProcess
/// \ingroup Graphics
///
/// This is the base class for all post processing effects.
///
///////////////////////////////////////////////////////////