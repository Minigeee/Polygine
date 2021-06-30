#ifndef POLY_RENDER_SYSTEM_H
#define POLY_RENDER_SYSTEM_H

#include <poly/Core/NonCopyable.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/FrameBuffer.h>

namespace poly
{

class Scene;
class Shader;


///////////////////////////////////////////////////////////
/// \brief An enum defining render passes
///
/// Render systems can decide to skip rendering for certain
/// render passes (i.e. skipping detail foliage for reflections).
///
///////////////////////////////////////////////////////////
enum class RenderPass
{
	Default			= 1 << 0,	//!< A default render pass
	Shadow			= 1 << 1,	//!< A shadow render pass
	Reflection		= 1 << 2,	//!< A reflection render pass
	Refraction		= 1 << 3,	//!< A refraction render pass

	All				= Default | Shadow | Reflection | Refraction
};

///////////////////////////////////////////////////////////
/// \brief The binary AND operator for render pass enums
///
/// \param a The first operand
/// \param b The second operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
RenderPass operator&(RenderPass a, RenderPass b);

///////////////////////////////////////////////////////////
/// \brief The binary OR operator for render pass enums
///
/// \param a The first operand
/// \param b The second operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
RenderPass operator|(RenderPass a, RenderPass b);

///////////////////////////////////////////////////////////
/// \brief The binary NOT operator for render pass enums
///
/// \param a The operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
RenderPass operator~(RenderPass a);


///////////////////////////////////////////////////////////
/// \brief The base class for all rendering procedure classes
///
///////////////////////////////////////////////////////////
class RenderSystem : public NonMoveable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default destructor
	///
	///////////////////////////////////////////////////////////
	RenderSystem();

	///////////////////////////////////////////////////////////
	/// \brief This function should initialize anything that is scene dependent,
	///        such as access to entities
	///
	/// \param scene A pointer to the scene to initialize the system for
	///
	///////////////////////////////////////////////////////////
	virtual void init(Scene* scene) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Execute the rendering procedures
	///
	/// \param camera The camera to render from the perspective of
	/// \param pass The render pass that is being executed
	///
	///////////////////////////////////////////////////////////
	virtual void render(Camera& camera, RenderPass pass) = 0;

protected:
	Scene* m_scene;		//!< A pointer to the scene
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::RenderSystem
/// \ingroup Graphics
///
/// The RenderSystem is the base class for any class that needs
/// a custom rendering procedure, such as terrain, the skybox,
/// a spatial structure for rendering (the octree), and anything
/// else that needs custom render procedures. Advanced knowledge
/// of OpenGL will be helpful if custom render systems are needed.
///
/// Look at the source code of Octree, Terrain, or Skybox for examples
/// of how to implement a render system.
///
///////////////////////////////////////////////////////////