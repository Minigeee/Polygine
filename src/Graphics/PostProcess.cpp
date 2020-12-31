#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/PostProcess.h>

namespace poly
{


///////////////////////////////////////////////////////////
VertexArray PostProcess::quadVao;

///////////////////////////////////////////////////////////
VertexBuffer PostProcess::quadVbo;

///////////////////////////////////////////////////////////
Shader ColorAdjust::s_shader;


///////////////////////////////////////////////////////////
VertexArray& PostProcess::getVertexArray()
{
	if (!quadVao.getId())
	{
		float vertices[] =
		{
			-1.0f,  1.0f,
			-1.0f, -1.0f,
			 1.0f,  1.0f,

			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
		};

		// Create vertex buffer
		quadVbo.create(vertices, 12);

		// Create vertex array
		quadVao.bind();
		quadVao.addBuffer(quadVbo, 0, 2);
	}

	return quadVao;
}


///////////////////////////////////////////////////////////
ColorAdjust::ColorAdjust() :
	m_gamma			(2.2f)
{

}


///////////////////////////////////////////////////////////
ColorAdjust::ColorAdjust(float gamma) :
	m_gamma			(gamma)
{

}


///////////////////////////////////////////////////////////
void ColorAdjust::render(FrameBuffer& input, FrameBuffer& output)
{
	// Bind output target
	output.bind();

	// Disable depth test
	glCheck(glDisable(GL_DEPTH_TEST));

	// Disable cull face
	glCheck(glDisable(GL_CULL_FACE));

	// Clear color buffer
	glCheck(glClear(GL_COLOR_BUFFER_BIT));

	// Bind shader
	Shader& shader = getShader();

	shader.bind();
	shader.setUniform("u_texture", 0);
	shader.setUniform("u_gamma", m_gamma);

	// Bind texture
	input.getColorTexture()->bind(0);

	// Render vertex array
	VertexArray& vao = PostProcess::getVertexArray();
	vao.bind();
	vao.draw();
}


///////////////////////////////////////////////////////////
Shader& ColorAdjust::getShader()
{
	if (!s_shader.getId())
	{
		// Load shader
		s_shader.load("shaders/postprocess/quad.vert", Shader::Vertex);
		s_shader.load("shaders/postprocess/color_adjust.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


///////////////////////////////////////////////////////////
void ColorAdjust::setGamma(float gamma)
{
	m_gamma = gamma;
}


///////////////////////////////////////////////////////////
float ColorAdjust::getGamma() const
{
	return m_gamma;
}


}