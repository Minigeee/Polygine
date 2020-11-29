#ifndef POLY_GRAPHICS_COMPONENTS_H
#define POLY_GRAPHICS_COMPONENTS_H

namespace poly
{

class Model;
class Shader;

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

	Model* m_model;		//!< The model to render
	Shader* m_shader;	//!< The shader to render the model with
	bool m_culled;		//!< This will be true if the entity is being culled
};


}

#endif