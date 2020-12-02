#ifndef POLY_POST_PROCESS_H
#define POLY_POST_PROCESS_H

#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief TODO
///
///////////////////////////////////////////////////////////
class PostProcess
{
public:
	virtual void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) = 0;

private:
	static VertexArray quadVao;
	static VertexBuffer quadVbo;

protected:
	static VertexArray& getVertexArray();
};


///////////////////////////////////////////////////////////
/// \brief TODO
///
///////////////////////////////////////////////////////////
class ColorAdjust : public PostProcess
{
public:
	ColorAdjust();

	ColorAdjust(float gamma);

	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	void setGamma(float gamma);

	float getGamma() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	float m_gamma;
};

}

#endif