#ifndef POLY_BOUNDING_BOX_H
#define POLY_BOUNDING_BOX_H

#include <poly/Math/Vector3.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A class representing a 3D axis-aligned bounding box
///
///////////////////////////////////////////////////////////
class BoundingBox
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	BoundingBox();

	///////////////////////////////////////////////////////////
	/// \brief Create a bounding box from a minimum coordinate and a maximum coordinate
	///
	/// \param min The minimum coordinate
	/// \param max The maximum coordinate
	///
	///////////////////////////////////////////////////////////
	BoundingBox(const Vector3f& min, const Vector3f& max);

	///////////////////////////////////////////////////////////
	/// \brief Get the center coordinate of the bounding box
	///
	/// \return The center coordinate
	///
	///////////////////////////////////////////////////////////
	Vector3f getCenter() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the size of the bounding box
	///
	/// \return The size or dimentions of the bounding box
	///
	///////////////////////////////////////////////////////////
	Vector3f getDimensions() const;

	Vector3f m_min;		//!< The minimum coordinate of the box
	Vector3f m_max;		//!< The maximum coordinate of the box
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::BoundingBox
/// \ingroup Math
///
/// The same thing as a 3D axis-aligned regtangular prsim.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a bounding box of size (2, 2, 2) centered at the origin
/// BoundingBox box(Vector3f(-1.0f), Vector3f(1.0f));
///
/// box.getCenter();		// (0, 0, 0)
/// box.getDimensions();	// (2, 2, 2)
///
/// \endcode
///
///////////////////////////////////////////////////////////