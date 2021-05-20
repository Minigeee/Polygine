#ifndef POLY_RENDERABLE_H
#define POLY_RENDERABLE_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/VertexArray.h>

#include <poly/Math/BoundingBox.h>
#include <poly/Math/Sphere.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief The base class for renderable objects
///
///////////////////////////////////////////////////////////
class Renderable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~Renderable();

	///////////////////////////////////////////////////////////
	/// \brief Get the local bounding box
	///
	/// \return The bounding box
	///
	///////////////////////////////////////////////////////////
	virtual const BoundingBox& getBoundingBox() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the local bounding sphere
	///
	/// \brief The bounding sphere
	///
	///////////////////////////////////////////////////////////
	virtual const Sphere& getBoundingSphere() const;

protected:
	BoundingBox m_boundingBox;			//!< The bounding box surrounding the model
	Sphere m_boundingSphere;			//!< The bounding sphere surrounding the model
};

}

#endif