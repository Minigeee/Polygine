#ifndef POLY_ENGINE_COMPONENTS_H
#define POLY_ENGINE_COMPONENTS_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief An engine components that describes an entity's
///	       position, rotation, and scale
/// \ingroup Components
///
///////////////////////////////////////////////////////////
struct TransformComponent
{
	///////////////////////////////////////////////////////////
	/// \brief Default transform
	///
	///////////////////////////////////////////////////////////
	TransformComponent() :
		m_position	(0.0f),
		m_scale		(1.0f)
	{ }

	Vector3f m_position;	// The position
	Quaternion m_rotation;	// The rotation
	Vector3f m_scale;		// The scale
};

}

#endif