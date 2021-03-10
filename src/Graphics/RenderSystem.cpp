#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
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
RenderSystem::RenderSystem() :
	m_scene		(0)
{

}


}