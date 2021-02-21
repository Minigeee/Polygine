#ifndef POLY_GRAPHICS_COMPONENTS_H
#define POLY_GRAPHICS_COMPONENTS_H

#include <poly/Math/Vector3.h>

namespace poly
{

class Animation;
class Renderable;
class Shader;
class Skeleton;

///////////////////////////////////////////////////////////
/// \brief A component that contains data that describes how to
///        render an entity
/// \ingroup Components
///
///////////////////////////////////////////////////////////
struct RenderComponent
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	RenderComponent();

	///////////////////////////////////////////////////////////
	/// \brief Create the render component from a model, and a shader
	///
	/// If no shader is provided, the default shader will be used
	///
	///////////////////////////////////////////////////////////
	RenderComponent(Renderable* renderable, Shader* shader = 0);

	Renderable* m_renderable;		//!< The model to render
	Shader* m_shader;				//!< The shader to render the model with
};


///////////////////////////////////////////////////////////
/// \brief A component that contains a skeleton that can have an animation
///	       applied to it. This will apply the skeleton to entities that also
///        contain a render component.
/// \ingroup Components
///
///////////////////////////////////////////////////////////
struct AnimationComponent
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	AnimationComponent();

	///////////////////////////////////////////////////////////
	/// \brief Create the component from a pointer to a skeleton
	///
	/// \param skeleton A pointer to a skeleton object
	///
	///////////////////////////////////////////////////////////
	AnimationComponent(Skeleton* skeleton);

	Skeleton* m_skeleton;	//!< The skeleton to apply to a render component
};


///////////////////////////////////////////////////////////
/// \brief A component that defines properties of a directional light
/// \ingroup Components
///
///////////////////////////////////////////////////////////
struct DirLightComponent
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates a light with white diffuse and specular colors, that
	/// points in the negative y-axis.
	///
	///////////////////////////////////////////////////////////
	DirLightComponent();

	Vector3f m_diffuse;		//!< The color of the diffuse lighting
	Vector3f m_specular;	//!< The color of the specular lighting
	Vector3f m_direction;	//!< The direction of the light
};


///////////////////////////////////////////////////////////
/// \brief Dynamic renderable component tag
/// \ingroup Components
///
/// Notifies octree that the entity will be updated many times.
///
///////////////////////////////////////////////////////////
struct DynamicTag { };


}

#endif