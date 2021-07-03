#include <poly/Core/ObjectPool.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Renderer.h>

#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Lighting.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shadows.h>

#include <poly/Graphics/Shaders/postprocess/quad.vert.h>
#include <poly/Graphics/Shaders/deferred.frag.h>

namespace poly
{


///////////////////////////////////////////////////////////
VertexArray FullscreenQuad::s_quadVao;

///////////////////////////////////////////////////////////
VertexBuffer FullscreenQuad::s_quadVbo;

///////////////////////////////////////////////////////////
Shader Renderer::s_deferredShader;


///////////////////////////////////////////////////////////
void FullscreenQuad::draw()
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

	s_quadVao.draw();
}


///////////////////////////////////////////////////////////
Renderer::Renderer(Scene* scene) :
	m_scene			(scene)
{

}


///////////////////////////////////////////////////////////
void Renderer::addRenderSystem(RenderSystem* system)
{
	m_renderSystems.push_back(system);
}


///////////////////////////////////////////////////////////
void Renderer::render(Camera& camera, FrameBuffer& target, RenderPass pass)
{
	FrameBuffer* buffer = 0;

	// If on a shadow pass, render directly to target buffer
	if (pass == RenderPass::Shadow)
		// Bind target buffer
		target.bind();

	else
	{
		// Check if the target has a mapping, or if its size changed
		auto it = m_gBuffers.find(target.getId());
		if (it == m_gBuffers.end())
		{
			// Create a new framebuffer
			buffer = m_gBuffers[target.getId()] = Pool<FrameBuffer>::alloc();

			buffer->create(target.getWidth(), target.getHeight());
			buffer->attachColor(Pool<Texture>::alloc(), PixelFormat::Rgba, GLType::HalfFloat);	//!< Normal + Shininess
			buffer->attachColor(Pool<Texture>::alloc(), PixelFormat::Rgba, GLType::Uint8);		//!< Albedo + Occlusion factor
			buffer->attachColor(Pool<Texture>::alloc(), PixelFormat::Rgba, GLType::Uint8);		//!< Specular + Reflectivity

			// Share depth buffer with target
			buffer->attachDepth(target.getDepthTexture());										//!< Depth
		}
		else if (it.value()->getWidth() != target.getWidth() || it.value()->getHeight() != target.getHeight())
		{
			buffer = it.value();

			// Get texture pointers before resest
			Texture* textures[] =
			{
				buffer->getColorTexture(0),
				buffer->getColorTexture(1),
				buffer->getColorTexture(2),
				buffer->getDepthTexture(),
			};

			// Reset buffer
			buffer->reset();

			buffer->create(target.getWidth(), target.getHeight());
			buffer->attachColor(textures[0], PixelFormat::Rgba, GLType::HalfFloat);	//!< Normal + Shininess
			buffer->attachColor(textures[1], PixelFormat::Rgba, GLType::Uint8);		//!< Albedo + Occlusion factor
			buffer->attachColor(textures[2], PixelFormat::Rgba, GLType::Uint8);		//!< Specular + Reflectivity
			buffer->attachDepth(textures[3]);										//!< Depth
			GL_HALF_FLOAT;
		}
		else
			buffer = it.value();

		// Bind deferred buffer
		buffer->bind();
	}

	// Clear buffers
	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Disable alpha blending
	glCheck(glDisable(GL_BLEND));

	// Deferred render
	for (Uint32 i = 0; i < m_renderSystems.size(); ++i)
	{
		RenderSystem* system = m_renderSystems[i];
		if (system->hasDeferredPass())
			system->render(camera, pass, true);
	}

	// Skip the rest of the render if doing a shadow pass
	if (pass == RenderPass::Shadow)
		return;


	// Make sure lighting is updated
	Lighting* lighting = m_scene->getExtension<Lighting>();
	lighting->update(camera);

	// Combine into target shader
	target.bind();

	// Clear only color buffer (depth was filled by previous renders)
	glCheck(glClear(GL_COLOR_BUFFER_BIT));

	// Disable depth testing
	glCheck(glDisable(GL_DEPTH_TEST));

	// Setup shader
	Shader& shader = getDeferredShader();
	shader.bind();

	// Bind all textures
	shader.setUniform("u_normalShininess", *buffer->getColorTexture(0));
	shader.setUniform("u_albedoOcclusion", *buffer->getColorTexture(1));
	shader.setUniform("u_specularReflectivity", *buffer->getColorTexture(2));
	shader.setUniform("u_depth", *buffer->getDepthTexture());
	shader.setUniform("u_invProjView", inverse(camera.getProjMatrix() * camera.getViewMatrix()));

	// Apply lighting and shadows
	camera.apply(&shader);
	lighting->apply(&shader);
	m_scene->getExtension<Shadows>()->apply(&shader);

	// Render lighting stage
	FullscreenQuad::draw();

	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));


	// Forward render
	for (Uint32 i = 0; i < m_renderSystems.size(); ++i)
	{
		RenderSystem* system = m_renderSystems[i];
		if (system->hasForwardPass())
			system->render(camera, pass, false);
	}
}


///////////////////////////////////////////////////////////
Shader& Renderer::getDeferredShader()
{
	if (!s_deferredShader.getId())
	{
		s_deferredShader.load("poly/postprocess/quad.vert", SHADER_POSTPROCESS_QUAD_VERT, Shader::Vertex);
		s_deferredShader.load("poly/deferred.frag", SHADER_DEFERRED_FRAG, Shader::Fragment);
		s_deferredShader.compile();
	}

	return s_deferredShader;
}


///////////////////////////////////////////////////////////
FrameBuffer* Renderer::getGBuffer(const FrameBuffer& target) const
{
	auto it = m_gBuffers.find(target.getId());
	return it == m_gBuffers.end() ? 0 : it.value();
}


}