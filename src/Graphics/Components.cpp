#include <poly/Graphics/Components.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent() :
	m_model		(0),
	m_shader	(0)
{

}


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent(Model* model, Shader* shader) :
	m_model		(model),
	m_shader	(shader)
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
	m_diffuse		(1.0f),
	m_specular		(1.0f),
	m_direction		(0.0f, -1.0f, 0.0f)
{

}


}