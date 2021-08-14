#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderPass operator&(RenderPass a, RenderPass b)
{
	return (RenderPass)((Uint32)a & (Uint32)b);
}


///////////////////////////////////////////////////////////
RenderPass operator|(RenderPass a, RenderPass b)
{
	return (RenderPass)((Uint32)a | (Uint32)b);
}


///////////////////////////////////////////////////////////
RenderPass operator~(RenderPass a)
{
	return (RenderPass)(~(Uint32)a);
}


///////////////////////////////////////////////////////////
RenderSettings::RenderSettings() :
	m_target			(0),
	m_numClipPlanes		(0),
	m_numPointLights	(INT32_MAX),
	m_deferred			(true)
{

}


///////////////////////////////////////////////////////////
RenderSystem::RenderSystem() :
	m_scene		(0)
{

}


///////////////////////////////////////////////////////////
bool RenderSystem::hasDeferredPass() const
{
	return true;
}


///////////////////////////////////////////////////////////
bool RenderSystem::hasForwardPass() const
{
	return false;
}


///////////////////////////////////////////////////////////
void RenderSystem::applyRenderSettings(Shader* shader, const RenderSettings& settings)
{
	// Enable clip planes
	for (Uint32 i = 0; i < settings.m_numClipPlanes; ++i)
	{
		glCheck(glEnable(GL_CLIP_DISTANCE0 + i));
		shader->setUniform("u_clipPlanes[" + std::to_string(i) + ']', settings.m_clipPlanes[i]);
	}

	shader->setUniform("u_numClipPlanes", (int)settings.m_numClipPlanes);
}


///////////////////////////////////////////////////////////
void RenderSystem::resetRenderSettings(const RenderSettings& settings)
{
	// Disable clip planes
	for (Uint32 i = 0; i < settings.m_numClipPlanes; ++i)
		glCheck(glDisable(GL_CLIP_DISTANCE0 + i));
}


}