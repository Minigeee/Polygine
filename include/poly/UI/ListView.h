#ifndef POLY_LIST_VIEW
#define POLY_LIST_VIEW

#include <poly/UI/UIContainer.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A vertical list view
///
///////////////////////////////////////////////////////////
class VListView : public UIContainer
{
public:
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	virtual void parse(XmlNode node, const UITemplateMap& templates = UITemplateMap());

	///////////////////////////////////////////////////////////
	/// \brief Append an element to the vertical list view
	///
	/// \param child A pointer to a child element
	///
	///////////////////////////////////////////////////////////
	void addChild(UIElement* child) override;

	///////////////////////////////////////////////////////////
	/// \brief Append an element to the vertical list view with a margin
	///
	/// \param child A pointer to a child element
	/// \param margins The element margins for before and after the element
	///
	///////////////////////////////////////////////////////////
	void addChild(UIElement* child, const Vector2f& margins);

	///////////////////////////////////////////////////////////
	/// \brief Remove a child element
	///
	/// \param child A child UI element
	///
	///////////////////////////////////////////////////////////
	void removeChild(UIElement* child);

	///////////////////////////////////////////////////////////
	/// \brief Recalculate the list view's bounds (and update the transforms of children element)
	///
	/// This function updates the transforms of all chidlren element,
	/// so that all elements will be in the right position, if
	/// the scale of any children element changed. Then the new
	/// container bounds are calculated (size and position).
	///
	///////////////////////////////////////////////////////////
	void updateBounds() override;

	///////////////////////////////////////////////////////////
	/// \brief Set the margins of a specific child element
	///
	/// This function sets the margins of a child element, and does
	/// nothing it the given element is not a child element. updateBounds()
	/// is automatically called so that all children elements have
	/// the correct positiona and the container size is correct.
	///
	/// \param element The element to set margins for
	/// \param margins The new element margins
	///
	///////////////////////////////////////////////////////////
	void setMargins(UIElement* element, const Vector2f& margins);

	///////////////////////////////////////////////////////////
	/// \brief Get a child element's margins
	///
	/// A zero vector is returned if the given element is not a
	/// child element.
	///
	/// \param element The child element to get margins for
	///
	/// \return The child element's margins
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getMargins(UIElement* element) const;

private:
	void getQuads(std::vector<UIQuad>& quads) override;

	virtual void updateTransforms() override;

private:
	std::vector<Vector2f> m_margins;
};


///////////////////////////////////////////////////////////
/// \brief A horizontal list view
///
///////////////////////////////////////////////////////////
class HListView : public UIContainer
{
public:
	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	virtual void parse(XmlNode node, const UITemplateMap& templates = UITemplateMap());

	///////////////////////////////////////////////////////////
	/// \brief Append an element to the horizontal list view
	///
	/// \param child A pointer to a child element
	///
	///////////////////////////////////////////////////////////
	void addChild(UIElement* child) override;

	///////////////////////////////////////////////////////////
	/// \brief Append an element to the horizontal list view with a margin
	///
	/// \param child A pointer to a child element
	/// \param margins The element margins for before and after the element
	///
	///////////////////////////////////////////////////////////
	void addChild(UIElement* child, const Vector2f& margins);

	///////////////////////////////////////////////////////////
	/// \brief Remove a child element
	///
	/// \param child A child UI element
	///
	///////////////////////////////////////////////////////////
	void removeChild(UIElement* child);

	///////////////////////////////////////////////////////////
	/// \brief Recalculate the list view's bounds (and update the transforms of children element)
	///
	/// This function updates the transforms of all chidlren element,
	/// so that all elements will be in the right position, if
	/// the scale of any children element changed. Then the new
	/// container bounds are calculated (size and position).
	///
	///////////////////////////////////////////////////////////
	void updateBounds() override;

	///////////////////////////////////////////////////////////
	/// \brief Set the margins of a specific child element
	///
	/// This function sets the margins of a child element, and does
	/// nothing it the given element is not a child element. updateBounds()
	/// is automatically called so that all children elements have
	/// the correct positiona and the container size is correct.
	///
	/// \param element The element to set margins for
	/// \param margins The new element margins
	///
	///////////////////////////////////////////////////////////
	void setMargins(UIElement* element, const Vector2f& margins);

	///////////////////////////////////////////////////////////
	/// \brief Get a child element's margins
	///
	/// A zero vector is returned if the given element is not a
	/// child element.
	///
	/// \param element The child element to get margins for
	///
	/// \return The child element's margins
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getMargins(UIElement* element) const;

private:
	void getQuads(std::vector<UIQuad>& quads) override;

	virtual void updateTransforms() override;

private:
	std::vector<Vector2f> m_margins;
};


///////////////////////////////////////////////////////////
/// \brief A vertical list view
///
///////////////////////////////////////////////////////////
typedef VListView ListView;

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::VListView
/// \ingroup UI
///
/// This element organizes its children element into a vertical
/// list, with optional margins between each element. The list
/// view calculates offsets based on the element's size at the
/// time the element is added as a child. So after adding an
/// element to a list view, it is not possible to recalculate
/// offsets to account for any changes in size or rotation.
/// The element offsets do not take rotation into account.
///
/// Usage example:
/// \code
///
/// using namespace poly
///
/// VListView list;
/// list.setPosition(30.0f, 30.0f);
///
/// // Add list elements
/// Text text[10];
/// for (Uint32 i = 0; i < 10; ++i)
/// {
///		texts[i].setString("Hello World!");
///		list.addChild(&texts[i]);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \class poly::HListView
/// \ingroup UI
///
/// This element organizes its children element into a horizontal
/// list, with optional margins between each element. The list
/// view calculates offsets based on the element's size at the
/// time the element is added as a child. So after adding an
/// element to a list view, it is not possible to recalculate
/// offsets to account for any changes in size or rotation.
/// The element offsets do not take rotation into account.
///
/// Usage example:
/// \code
///
/// using namespace poly
///
/// HListView list;
/// list.setPosition(30.0f, 30.0f);
///
/// // Add list elements
/// Text text[10];
/// for (Uint32 i = 0; i < 10; ++i)
/// {
///		texts[i].setString("Hello World!");
///		list.addChild(&texts[i]);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////