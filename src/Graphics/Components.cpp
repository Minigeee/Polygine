#include <poly/Graphics/Components.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent() :
	m_renderable	(0),
	m_shader		(0)
{

}


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent(Renderable* renderable, Shader* shader) :
	m_renderable	(renderable),
	m_shader		(shader)
{

}


///////////////////////////////////////////////////////////
AnimationComponent::AnimationComponent() :
	m_skeleton		(0)
{

}


///////////////////////////////////////////////////////////
AnimationComponent::AnimationComponent(Skeleton* skeleton) :
	m_skeleton		(skeleton)
{

}


///////////////////////////////////////////////////////////
DirLightComponent::DirLightComponent() :
	m_diffuse				(1.0f),
	m_specular				(1.0f),
	m_direction				(0.0f, -1.0f, 0.0f),
	m_shadowResolution		(2048),
	m_shadowDistance		(10.0f),
	m_shadowStrength		(0.8f),
	m_shadowsEnabled		(true)
{

}


}