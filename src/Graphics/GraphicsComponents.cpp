#include <poly/Graphics/Components.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent() :
	m_renderable	(0),
	m_castsShadows	(true)
{

}


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent(Renderable* renderable) :
	m_renderable	(renderable),
	m_castsShadows	(true)
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
	m_shadowDistance		(50.0f),
	m_shadowStrength		(1.0f),
	m_shadowCascades		(3),
	m_cascadeDistMultiplier	(1.5f),
	m_shadowsEnabled		(true)
{

}


///////////////////////////////////////////////////////////
PointLightComponent::PointLightComponent() :
	m_diffuse				(1.0f),
	m_specular				(1.0f),
	m_coefficients			(1.0f, 0.35f, 0.44f)
{

}


}