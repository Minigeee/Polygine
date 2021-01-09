#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
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
Shader Fog::s_shader;

///////////////////////////////////////////////////////////
Shader Fxaa::s_shader;


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

	// Bind shader
	Shader& shader = getShader();

	shader.bind();
	shader.setUniform("u_texture", *input.getColorTexture());
	shader.setUniform("u_gamma", m_gamma);

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


///////////////////////////////////////////////////////////
Fog::Fog() :
	m_scene				(0),
	m_camera			(0),
	m_color				(0.272f, 0.548f, 0.675f),
	m_density			(0.0005f),
	m_scatterStrength	(0.0f),
	m_applyToSkybox		(true)
{

}


///////////////////////////////////////////////////////////
void Fog::render(FrameBuffer& input, FrameBuffer& output)
{
	// Bind output target
	output.bind();

	// Disable depth test
	glCheck(glDisable(GL_DEPTH_TEST));

	// Disable cull face
	glCheck(glDisable(GL_CULL_FACE));

	// Bind shader
	Shader& shader = getShader();

	shader.bind();
	shader.setUniform("u_cameraPos", m_camera->getPosition());
	shader.setUniform("u_color", *input.getColorTexture());
	shader.setUniform("u_depth", *input.getDepthTexture());
	shader.setUniform("u_fogColor", m_color);
	shader.setUniform("u_fogDensity", m_density);
	shader.setUniform("u_scatterStrength", m_scatterStrength);
	shader.setUniform("u_applyToSkybox", (int)m_applyToSkybox);

	if (m_scene)
	{
		Vector3f lightDir(0.0);

		int i = 0;
		m_scene->system<DirLightComponent>(
			[&](const Entity::Id id, DirLightComponent& light)
			{
				if (i++ == 0)
				{
					lightDir = normalize(light.m_direction);
					shader.setUniform("u_lightDir", lightDir);
					shader.setUniform("u_lightColor", light.m_diffuse);
				}
			}
		);

		if (i > 0)
		{
			shader.setUniform("u_invProjView", inverse(m_camera->getProjMatrix() * m_camera->getViewMatrix()));
		}
	}

	// Render vertex array
	VertexArray& vao = PostProcess::getVertexArray();
	vao.bind();
	vao.draw();
}


///////////////////////////////////////////////////////////
void Fog::setScene(Scene* scene)
{
	m_scene = scene;
}


///////////////////////////////////////////////////////////
void Fog::setCamera(Camera* camera)
{
	m_camera = camera;
}


///////////////////////////////////////////////////////////
void Fog::setColor(float r, float g, float b)
{
	m_color = Vector3f(r, g, b);
}


///////////////////////////////////////////////////////////
void Fog::setDensity(float density)
{
	m_density = density;
}


///////////////////////////////////////////////////////////
void Fog::setScatterStrength(float strength)
{
	m_scatterStrength = strength;
}


///////////////////////////////////////////////////////////
void Fog::setSkyboxFog(bool skybox)
{
	m_applyToSkybox = skybox;
}


///////////////////////////////////////////////////////////
const Vector3f& Fog::getColor() const
{
	return m_color;
}


///////////////////////////////////////////////////////////
float Fog::getDensity() const
{
	return m_density;
}


///////////////////////////////////////////////////////////
float Fog::getScatterStrength() const
{
	return m_scatterStrength;
}


///////////////////////////////////////////////////////////
Shader& Fog::getShader()
{
	if (!s_shader.getId())
	{
		// Load shader
		s_shader.load("shaders/postprocess/quad.vert", Shader::Vertex);
		s_shader.load("shaders/postprocess/fog.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


///////////////////////////////////////////////////////////
Fxaa::Fxaa() :
	m_threshold		(0.1f)
{

}


///////////////////////////////////////////////////////////
void Fxaa::setLumaThreshold(float threshold)
{
	m_threshold = threshold;
}


///////////////////////////////////////////////////////////
float Fxaa::getLumaThreshold() const
{
	return m_threshold;
}



///////////////////////////////////////////////////////////
void Fxaa::render(FrameBuffer& input, FrameBuffer& output)
{
	// Bind output target
	output.bind();

	// Disable depth test
	glCheck(glDisable(GL_DEPTH_TEST));

	// Disable cull face
	glCheck(glDisable(GL_CULL_FACE));

	// Bind shader
	Shader& shader = getShader();

	shader.bind();
	shader.setUniform("u_colorTexture", *input.getColorTexture());
	shader.setUniform("u_texelStep", 1.0f / Vector2f(input.getWidth(), input.getHeight()));
	shader.setUniform("u_lumaThreshold", m_threshold);

	// Render vertex array
	VertexArray& vao = PostProcess::getVertexArray();
	vao.bind();
	vao.draw();
}


///////////////////////////////////////////////////////////
Shader& Fxaa::getShader()
{
	if (!s_shader.getId())
	{
		// Load shader
		s_shader.load("shaders/postprocess/quad.vert", Shader::Vertex);
		s_shader.load("shaders/postprocess/fxaa.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


}