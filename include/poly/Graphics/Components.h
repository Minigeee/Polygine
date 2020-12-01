#ifndef POLY_GRAPHICS_COMPONENTS_H
#define POLY_GRAPHICS_COMPONENTS_H

#include <poly/Math/Vector3.h>

namespace poly
{

class Model;
class Shader;
class Skeleton;

///////////////////////////////////////////////////////////
/// \brief A component that contains data that describes how to
///        render an entity
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
	RenderComponent(Model* model, Shader* shader = 0);

	Model* m_model;			//!< The model to render
	Shader* m_shader;		//!< The shader to render the model with
	Skeleton* m_skeleton;	//!< The skeleton to apply to the model
};


///////////////////////////////////////////////////////////
/// \brief A component that defines properties of a directional light
///
///////////////////////////////////////////////////////////
struct DirLightComponent
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	DirLightComponent();

	Vector3f m_diffuse;		//!< The color of the diffuse lighting
	Vector3f m_specular;	//!< The color of the specular lighting
	Vector3f m_direction;	//!< The direction of the light
};


}

#endif