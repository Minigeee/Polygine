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
/// \brief A struct containing render settings
///
///////////////////////////////////////////////////////////
struct RenderSettings
{
	RenderSettings();

	FrameBuffer* m_target;		//!< This property will be set by the renderer (the user can leave this as NULL)
	Vector4f m_clipPlanes[8];	//!< An array of clip planes
	Uint32 m_numClipPlanes;		//!< The number of enabled clip planes
	Uint32 m_numPointLights;	//!< The number of point lights to enable (capped by the maximum allowed by light system)
	bool m_deferred;			//!< Determines if the system should render using deferred render or forward render
};


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
	/// Render systems can optionally handle scenarios that require
	/// deferred or forward rendering. To enable forward rendering,
	/// override hasForwardPass() to return true, and to disable deferred
	/// rendering, override hasDeferredPass() to return false. The
	/// deferred render will always be executed before the forward
	/// render.
	///
	/// For rendering objects with colors outside the range of 0 to 1, use forward
	/// rendering and render into a 16-bit framebuffer because deferred
	/// rendering only uses 8-bit color buffers for performance reasons.
	///
	/// \param camera The camera to render from the perspective of
	/// \param pass The render pass that is being executed
	/// \param settings The render settings that control the behavior of the render system
	///
	///////////////////////////////////////////////////////////
	virtual void render(Camera& camera, RenderPass pass, const RenderSettings& settings) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Check if the render system has a deferred render pass
	///
	/// Override this to return false to disable deferred rendering
	/// for this render system.
	///
	/// \return True if this system handles deferred rendering
	///
	///////////////////////////////////////////////////////////
	virtual bool hasDeferredPass() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the render system has a forward render pass
	///
	/// Override this to return true to enable forward rendering
	/// for this render system.
	///
	/// \return True if this system handles forward rendering
	///
	///////////////////////////////////////////////////////////
	virtual bool hasForwardPass() const;

protected:
	///////////////////////////////////////////////////////////
	/// \brief A convenient utility function to quickly apply render settings
	///
	/// \param shader A pointer to the shader to apply the settings to
	/// \param settings The settings to apply
	///
	///////////////////////////////////////////////////////////
	void applyRenderSettings(Shader* shader, const RenderSettings& settings);

	///////////////////////////////////////////////////////////
	/// \brief A convenient utility function to quickly reset render settings
	///
	/// \param settings The settings to reset
	///
	///////////////////////////////////////////////////////////
	void resetRenderSettings(const RenderSettings& settings);

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