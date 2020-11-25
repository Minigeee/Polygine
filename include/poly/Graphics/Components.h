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
	Model* m_model;		//!< The model to render
	Shader* m_shader;	//!< The shader to render the model with
	bool m_culled;		//!< This will be true if the entity is being culled
};


}

#endif