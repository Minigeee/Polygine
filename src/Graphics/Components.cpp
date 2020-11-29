#include <poly/Graphics/Components.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent() :
	m_model		(0),
	m_shader	(0),
	m_culled	(false)
{

}


///////////////////////////////////////////////////////////
RenderComponent::RenderComponent(Model* model, Shader* shader) :
	m_model		(model),
	m_shader	(shader),
	m_culled	(false)
{

}


}