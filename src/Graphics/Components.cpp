#include <poly/Graphics/Components.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent() :
	m_model		(0),
	m_shader	(0),
	m_skeleton	(0)
{

}


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent(Model* model, Shader* shader) :
	m_model		(model),
	m_shader	(shader),
	m_skeleton	(0)
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