#ifndef POLY_UI_CONTAINER_H
#define POLY_UI_CONTAINER_H

#include <poly/UI/UIElement.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief The base class for container type UI elements
///
///////////////////////////////////////////////////////////
class UIContainer : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~UIContainer() { }

	///////////////////////////////////////////////////////////
	/// \brief Update the container bounds
	///
	/// Containers are meant to hold many elements, so this function
	/// is meant to recalculate its internal bounds based on the
	/// transforms of the containers internal elements. Elements don't
	/// have access to their children's \a transformChanged property,
	/// so it is not possible to check if its transforms have been
	/// changed each time the container's quads are requested.
	/// Instead, this function should be called whenever it is known
	/// that one of the children elements had their bounds changed.
	///
	/// Note that this function should only update the container bounds,
	/// not the element bounds. In most cases, the container bounds is
	/// equal to the element bounds, but some elemnts, such as the
	/// ScrollView have different container bounds and element bounds.
	///
	///////////////////////////////////////////////////////////
	virtual void updateBounds() = 0;
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::UIContainer
/// \ingroup UI
///
/// This class only provides the updateBounds() function which
/// is meant to be used to recalculate the bounds of a container
/// based on the elements inside it.
///
///////////////////////////////////////////////////////////