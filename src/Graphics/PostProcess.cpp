#include <poly/Core/ObjectPool.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/PostProcess.h>

namespace poly
{


///////////////////////////////////////////////////////////
VertexArray PostProcess::s_quadVao;

///////////////////////////////////////////////////////////
VertexBuffer PostProcess::s_quadVbo;

///////////////////////////////////////////////////////////
Shader ColorAdjust::s_shader;

///////////////////////////////////////////////////////////
Shader Fog::s_shader;

///////////////////////////////////////////////////////////
Shader Fxaa::s_shader;

///////////////////////////////////////////////////////////
Shader Blur::s_shader;

///////////////////////////////////////////////////////////
Shader Bloom::s_thresholdShader;

///////////////////////////////////////////////////////////
Shader Bloom::s_addShader;

///////////////////////////////////////////////////////////
Shader Ssao::s_shader;


///////////////////////////////////////////////////////////
VertexArray& PostProcess::getVertexArray()
{
	if (!s_quadVao.getId())
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
		s_quadVbo.create(vertices, 12);

		// Create vertex array
		s_quadVao.bind();
		s_quadVao.addBuffer(s_quadVbo, 0, 2);
	}

	return s_quadVao;
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
	m_depthTexture		(0),
	m_color				(0.272f, 0.548f, 0.675f),
	m_density			(0.0005f),
	m_scatterStrength	(0.0f),
	m_applyToSkybox		(true)
{

}


///////////////////////////////////////////////////////////
void Fog::render(FrameBuffer& input, FrameBuffer& output)
{
	if (!m_camera || !m_depthTexture)
		return;

	// Bind output target
	output.bind();

	// Disable depth test
	glCheck(glDisable(GL_DEPTH_TEST));

	// Disable cull face
	glCheck(glDisable(GL_CULL_FACE));

	// Bind shader
	Shader& shader = getShader();

	shader.bind();
	shader.setUniform("u_color", *input.getColorTexture());
	shader.setUniform("u_depth", *m_depthTexture);
	shader.setUniform("u_fogColor", m_color);
	shader.setUniform("u_fogDensity", m_density);
	shader.setUniform("u_scatterStrength", m_scatterStrength);
	shader.setUniform("u_applyToSkybox", (int)m_applyToSkybox);

	m_camera->apply(&shader);

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
void Fog::setDepthTexture(Texture* texture)
{
	m_depthTexture = texture;
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
	m_threshold			(0.1f)
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


///////////////////////////////////////////////////////////
Blur::Blur() :
	m_distType		(Gaussian),
	m_kernelSize	(11),
	m_kernelSpacing	(1.0f),
	m_noiseFactor	(1.0f),
	m_spread		(1.7f),
	m_verticalBlur	(true),
	m_paramsDirty	(true)
{

}


///////////////////////////////////////////////////////////
void Blur::render(FrameBuffer& input, FrameBuffer& output)
{
	// Bind output target
	output.bind();

	// Disable depth test
	glCheck(glDisable(GL_DEPTH_TEST));

	// Disable cull face
	glCheck(glDisable(GL_CULL_FACE));

	// Update distribution weights if needed
	if (m_paramsDirty)
	{
		m_weights.clear();

		if (m_distType == Uniform)
		{
			Uint32 numWeights = m_kernelSize / 2 + 1;
			float weight = 1.0f / m_kernelSize;

			for (Uint32 i = 0; i < numWeights; ++i)
				m_weights.push_back(weight);
		}
		else if (m_distType == Gaussian)
		{
			Uint32 numWeights = m_kernelSize / 2 + 1;
			float sum = 0.0f;

			for (Uint32 i = 0; i < numWeights; ++i)
			{
				m_weights.push_back(exp(-(float)i * i / (2.0f * m_spread * m_spread)));
				sum += m_weights.back() * (i == 0 ? 1.0f : 2.0f);
			}

			for (Uint32 i = 0; i < numWeights; ++i)
				m_weights[i] /= sum;
		}
	}

	// Bind shader
	Shader& shader = getShader();

	shader.bind();
	shader.setUniform("u_texture", *input.getColorTexture());

	shader.setUniform("u_verticalBlur", (int)m_verticalBlur);
	shader.setUniform("u_kernelSize", (int)m_kernelSize);
	shader.setUniform("u_kernelSpacing", m_kernelSpacing);
	shader.setUniform("u_noiseFactor", m_noiseFactor);
	for (Uint32 i = 0; i < m_weights.size(); ++i)
		shader.setUniform("u_weights[" + std::to_string(i) + ']', m_weights[i]);

	// Render vertex array
	VertexArray& vao = PostProcess::getVertexArray();
	vao.bind();
	vao.draw();
}


///////////////////////////////////////////////////////////
void Blur::setDistType(Blur::DistType type)
{
	m_distType = type;
	m_paramsDirty = true;
}


///////////////////////////////////////////////////////////
void Blur::setKernelSize(Uint32 size)
{
	m_kernelSize = size;
	m_paramsDirty = true;
}


///////////////////////////////////////////////////////////
void Blur::setKernelSpacing(float spacing)
{
	m_kernelSpacing = spacing;
}


///////////////////////////////////////////////////////////
void Blur::setNoiseFactor(float factor)
{
	m_noiseFactor = factor;
}


///////////////////////////////////////////////////////////
void Blur::setSpread(float spread)
{
	m_spread = spread;
	m_paramsDirty = true;
}


///////////////////////////////////////////////////////////
void Blur::setVerticalBlur(bool vertical)
{
	m_verticalBlur = vertical;
}


///////////////////////////////////////////////////////////
Blur::DistType Blur::getDistType() const
{
	return m_distType;
}


///////////////////////////////////////////////////////////
Uint32 Blur::getKernelSize() const
{
	return m_kernelSize;
}


///////////////////////////////////////////////////////////
float Blur::getKernelSpacing() const
{
	return m_kernelSpacing;
}


///////////////////////////////////////////////////////////
float Blur::getNoiseFactor() const
{
	return m_noiseFactor;
}


///////////////////////////////////////////////////////////
float Blur::getSpread() const
{
	return m_spread;
}


///////////////////////////////////////////////////////////
bool Blur::usesVerticalBlur() const
{
	return m_verticalBlur;
}


///////////////////////////////////////////////////////////
Shader& Blur::getShader()
{
	if (!s_shader.getId())
	{
		// Load shader
		s_shader.load("shaders/postprocess/quad.vert", Shader::Vertex);
		s_shader.load("shaders/postprocess/blur.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


///////////////////////////////////////////////////////////
Bloom::Bloom() :
	m_blurTarget			(0),
	m_blurTexture			(0),
	m_intensity				(2.0f),
	m_threshold				(1.0f),
	m_thresholdInterval		(0.8f),
	m_radius				(0.05f),
	m_numBlurs				(3)
{

}


///////////////////////////////////////////////////////////
Bloom::~Bloom()
{
	if (m_blurTexture)
		Pool<Texture>::free(m_blurTexture);

	if (m_blurTexture)
		Pool<FrameBuffer>::free(m_blurTarget);

	m_blurTexture = 0;
	m_blurTarget = 0;
}


///////////////////////////////////////////////////////////
void Bloom::render(FrameBuffer& input, FrameBuffer& output)
{
	// Create the blur framebuffer if it hasn't been created yet
	if (!m_blurTarget)
	{
		m_blurTarget = Pool<FrameBuffer>::alloc();
		m_blurTexture = Pool<Texture>::alloc();

		m_blurTarget->create(output.getWidth(), output.getHeight());
		m_blurTarget->attachColor(m_blurTexture, PixelFormat::Rgb, GLType::Uint16);

		// Update blur settings
		float spacing = output.getHeight() * m_radius / 11.0f;
		m_blurEffect.setKernelSize(11);
		m_blurEffect.setKernelSpacing(spacing);
		m_blurEffect.setSpread(3.75f);
	}

	// Disable depth test
	glCheck(glDisable(GL_DEPTH_TEST));

	// Disable cull face
	glCheck(glDisable(GL_CULL_FACE));

	// Get vertex array
	VertexArray& vao = PostProcess::getVertexArray();


	Shader& thresholdShader = getThresholdShader();
	Shader& addShader = getAddShader();
	Uint32 currentTarget = 0;

	// Render threshold stage
	m_blurTarget->bind();

	thresholdShader.bind();
	thresholdShader.setUniform("u_texture", *input.getColorTexture());
	thresholdShader.setUniform("u_threshold", m_threshold);
	thresholdShader.setUniform("u_interval", m_thresholdInterval);
	vao.draw();

	// Blur the threshold texture
	for (Uint32 i = 0; i < m_numBlurs; ++i)
	{
		m_blurEffect.setVerticalBlur(false);
		m_blurEffect.render(*m_blurTarget, output);
		m_blurEffect.setVerticalBlur(true);
		m_blurEffect.render(output, *m_blurTarget);
	}

	// Render the bloom effect
	output.bind();

	addShader.bind();
	addShader.setUniform("u_texture1", *input.getColorTexture());
	addShader.setUniform("u_texture2", *m_blurTarget->getColorTexture());
	addShader.setUniform("u_factor1", 1.0f);
	addShader.setUniform("u_factor2", m_intensity);
	vao.draw();
}


///////////////////////////////////////////////////////////
void Bloom::setIntensity(float intensity)
{
	m_intensity = intensity;
}


///////////////////////////////////////////////////////////
void Bloom::setThreshold(float threshold)
{
	m_threshold = threshold;
}


///////////////////////////////////////////////////////////
void Bloom::setThresholdInterval(float interaval)
{
	m_thresholdInterval = interaval;
}


///////////////////////////////////////////////////////////
void Bloom::setRadius(float radius)
{
	m_radius = radius;
}


///////////////////////////////////////////////////////////
void Bloom::setNumBlurs(Uint32 numBlurs)
{
	m_numBlurs = numBlurs;
}


///////////////////////////////////////////////////////////
float Bloom::getIntensity() const
{
	return m_intensity;
}


///////////////////////////////////////////////////////////
float Bloom::getThreshold() const
{
	return m_threshold;
}


///////////////////////////////////////////////////////////
float Bloom::getThresholdInterval() const
{
	return m_thresholdInterval;
}


///////////////////////////////////////////////////////////
float Bloom::getRadius() const
{
	return m_radius;
}


///////////////////////////////////////////////////////////
Uint32 Bloom::getNumBlurs() const
{
	return m_numBlurs;
}


///////////////////////////////////////////////////////////
Shader& Bloom::getThresholdShader()
{
	if (!s_thresholdShader.getId())
	{
		s_thresholdShader.load("shaders/postprocess/quad.vert", Shader::Vertex);
		s_thresholdShader.load("shaders/postprocess/threshold.frag", Shader::Fragment);
		s_thresholdShader.compile();
	}

	return s_thresholdShader;
}


///////////////////////////////////////////////////////////
Shader& Bloom::getAddShader()
{
	if (!s_addShader.getId())
	{
		s_addShader.load("shaders/postprocess/quad.vert", Shader::Vertex);
		s_addShader.load("shaders/postprocess/add.frag", Shader::Fragment);
		s_addShader.compile();
	}

	return s_addShader;
}


///////////////////////////////////////////////////////////
Ssao::Ssao() :
	m_camera			(0),
	m_depthTexture		(0),
	m_radius			(0.2f),
	m_bias				(0.0f),
	m_range				(30.0f),
	m_falloff			(0.1f),
	m_intensity			(0.8f),
	m_noiseFactor		(0.1f)
{

}



///////////////////////////////////////////////////////////
void Ssao::render(FrameBuffer& input, FrameBuffer& output)
{
	// A depth texture and camera are required for SSAO to be used
	if (!m_camera || !m_depthTexture) return;

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
	shader.setUniform("u_depthTexture", *m_depthTexture);
	shader.setUniform("u_radius", m_radius);
	shader.setUniform("u_bias", m_bias);
	shader.setUniform("u_range", m_range);
	shader.setUniform("u_falloff", m_falloff);
	shader.setUniform("u_intensity", m_intensity);
	shader.setUniform("u_noiseFactor", m_noiseFactor);

	m_camera->apply(&shader);
	shader.setUniform("u_invProjView", inverse(m_camera->getProjMatrix() * m_camera->getViewMatrix()));

	// Render vertex array
	VertexArray& vao = PostProcess::getVertexArray();
	vao.bind();
	vao.draw();
}


///////////////////////////////////////////////////////////
void Ssao::setCamera(Camera* camera)
{
	m_camera = camera;
}


///////////////////////////////////////////////////////////
void Ssao::setDepthTexture(Texture* texture)
{
	m_depthTexture = texture;
}


///////////////////////////////////////////////////////////
void Ssao::setRadius(float radius)
{
	m_radius = radius;
}


///////////////////////////////////////////////////////////
void Ssao::setBias(float bias)
{
	m_bias = bias;
}


///////////////////////////////////////////////////////////
void Ssao::setRange(float range)
{
	m_range = range;
}


///////////////////////////////////////////////////////////
void Ssao::setFalloff(float falloff)
{
	m_falloff = falloff;
}


///////////////////////////////////////////////////////////
void Ssao::setIntensity(float intensity)
{
	m_intensity = intensity;
}


///////////////////////////////////////////////////////////
void Ssao::setNoiseFactor(float noiseFactor)
{
	m_noiseFactor = noiseFactor;
}


///////////////////////////////////////////////////////////
float Ssao::getRadius() const
{
	return m_radius;
}


///////////////////////////////////////////////////////////
float Ssao::getBias() const
{
	return m_bias;
}


///////////////////////////////////////////////////////////
float Ssao::getRange() const
{
	return m_range;
}


///////////////////////////////////////////////////////////
float Ssao::getFalloff() const
{
	return m_falloff;
}


///////////////////////////////////////////////////////////
float Ssao::getIntensity() const
{
	return m_intensity;
}


///////////////////////////////////////////////////////////
float Ssao::getNoiseFactor() const
{
	return m_noiseFactor;
}


///////////////////////////////////////////////////////////
Shader& Ssao::getShader()
{
	if (!s_shader.getId())
	{
		// Load shader
		s_shader.load("shaders/postprocess/quad.vert", Shader::Vertex);
		s_shader.load("shaders/postprocess/ssao.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


}