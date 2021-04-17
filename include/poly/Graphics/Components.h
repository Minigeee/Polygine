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
	bool m_castsShadows;			//!< Controls whether or not this render object casts shadows
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

	Vector3f m_diffuse;				//!< The color of the diffuse lighting
	Vector3f m_specular;			//!< The color of the specular lighting
	Vector3f m_direction;			//!< The direction of the light

	Uint32 m_shadowResolution;		//!< The base resolution of the shadow map
	float m_shadowDistance;			//!< The furthest distance shadows should be rendered
	float m_shadowStrength;			//!< The amount of light blocked by shadowed areas
	Uint32 m_shadowCascades;		//!< The number of shadow cascades to use
	float m_cascadeResMultiplier;	//!< The shadow map resolution multiplier between each cascade level
	float m_cascadeDistMultiplier;	//!< The shadow distance cutoff multplier between each cascade level
	bool m_shadowsEnabled;			//!< Flag that determines if this light should cast shadows or not
};


///////////////////////////////////////////////////////////
/// \brief A component that defines properties of a point light
/// \ingroup Components
///
/// A point light is a light that emits light in all directions
/// equally, and its intensity grows weaker as the distance from
/// the light increases. The fadeout of intensity can be controlled
/// with the 3 attenuation coefficients, where the x-component is the
/// \a constant coefficient, the y-component is the \a linear coefficient,
/// and the z-component is the \a quadratic coefficient.
///
/// \note The position of the point light can be set by attaching
/// a TransformComponent to the same entity that contains this
/// point light.
///
/// \note There is a maximum radius from the camera, where point
/// lights outside the range will be disabled. This is for performance
/// issues.
///
///////////////////////////////////////////////////////////
struct PointLightComponent
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates a point light component with white diffuse and
	/// specular color, and with the initial coefficient values
	/// of (1.0, 0.35, 0.44).
	///
	///////////////////////////////////////////////////////////
	PointLightComponent();

	Vector3f m_diffuse;				//!< The color of the diffuse lighting
	Vector3f m_specular;			//!< The color of the specular lighting
	Vector3f m_coefficients;		//!< The three attenuation coefficients
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