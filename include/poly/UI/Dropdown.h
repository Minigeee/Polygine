#ifndef POLY_DROPDOWN_H
#define POLY_DROPDOWN_H

#include <poly/UI/Button.h>

namespace poly
{

class VListView;

///////////////////////////////////////////////////////////
/// \brief A dropdown menu UI element
///
///////////////////////////////////////////////////////////
class Dropdown : public Button
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Dropdown();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~Dropdown();

	///////////////////////////////////////////////////////////
	/// \brief Parse properties for this UI element from an XML node
	///
	/// \see UIElement::parse
	///
	///////////////////////////////////////////////////////////
	virtual void parse(XmlNode node, const UITemplateMap& templates = UITemplateMap()) override;

	///////////////////////////////////////////////////////////
	/// \brief Add a dropdown menu item from a string
	///
	/// When adding an item, a new button is created and all its
	/// handlers will automatically be connected. It is not possible
	/// to customize each item button much besides changing its height
	/// and color, but onItemAdd() can be used for more customization
	/// by giving access to the item button.
	///
	/// \param name The name if the menu item
	///
	///////////////////////////////////////////////////////////
	void addItem(const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Add a dropdown menu item from an existing button
	///
	/// All trnasform properties, such as position, rotation, scale,
	/// origin, and anchor will be overriden by the dropdown menu.
	/// Color, texture, and text properties will be kept. This means
	/// that the default item color will not be applied to this
	/// button.
	///
	/// All button callbacks will be rebinded as well.
	///
	/// \param button The button to add as an item
	///
	///////////////////////////////////////////////////////////
	void addItem(Button* button);

	///////////////////////////////////////////////////////////
	/// \brief Remove an item from the dropdown menu
	///
	/// \param index The index of the item to remove
	///
	///////////////////////////////////////////////////////////
	void removeItem(Uint32 index);

	///////////////////////////////////////////////////////////
	/// \brief Clear all items from the dropdown menu
	///
	///////////////////////////////////////////////////////////
	void clearItems();

	///////////////////////////////////////////////////////////
	/// \brief Set the list of items
	///
	/// \param items The list of string dropdown items
	///
	///////////////////////////////////////////////////////////
	void setItems(const std::vector<std::string>& items);

	///////////////////////////////////////////////////////////
	/// \brief Set the selected item in the dropdown menu
	///
	/// \param index The index of the selected item
	///
	///////////////////////////////////////////////////////////
	void setSelectedItem(Uint32 index);

	///////////////////////////////////////////////////////////
	/// \brief Set the height of each item in pixels
	///
	/// This function does not affect items that have already been
	/// added.
	///
	/// \param height The height of each item in pixels
	///
	///////////////////////////////////////////////////////////
	void setItemHeight(float height);

	///////////////////////////////////////////////////////////
	/// \brief Set the default color of each item
	///
	/// \param color The default color of each item
	///
	///////////////////////////////////////////////////////////
	void setItemColor(const Vector4f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the default color of each item
	///
	/// \param r The r-component of the item color
	/// \param g The g-component of the item color
	/// \param b The b-component of the item color
	/// \param a The a-component of the item color
	///
	///////////////////////////////////////////////////////////
	void setItemColor(float r, float g, float b, float a);

	///////////////////////////////////////////////////////////
	/// \brief Set a repeating item color pattern
	///
	/// This can be used for alternating color dropdown lists,
	/// or dropdown lists with multiple repeating colors.
	///
	/// \param colors The list of repeating colors
	///
	///////////////////////////////////////////////////////////
	void setItemColorPattern(const std::vector<Vector4f>& colors);

	///////////////////////////////////////////////////////////
	/// \brief Overrided version of set visibility
	///
	/// Prevents the dropdown submenu from inheriting the visibility
	/// property.
	///
	/// \param visible Visibilty flag
	/// \param recursive Set this to true to apply to all subchildren
	///
	///////////////////////////////////////////////////////////
	void setVisible(bool visible, bool recursive = true) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the index of the selected item
	///
	/// \return The index of the selected item
	///
	///////////////////////////////////////////////////////////
	Uint32 getSelectedItem() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer of an item from its index
	///
	/// \param index The index of the item to retrieve
	///
	/// \return A pointer to an item button
	///
	///////////////////////////////////////////////////////////
	Button* getItem(Uint32 index);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called for every item that is added
	///
	/// When the callback function is set, it will be applied to
	/// to all existing items.
	///
	/// \param func A function callback
	///
	///////////////////////////////////////////////////////////
	void onItemAdd(const std::function<void(Button*, Uint32)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called every time the selected item changes
	///
	/// \param func A function callback
	///
	///////////////////////////////////////////////////////////
	void onItemChange(const std::function<void(Uint32)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the cursor enters an item button
	///
	/// \param func A function callback
	///
	///////////////////////////////////////////////////////////
	void onMouseEnterItem(const std::function<void(Button*, const E_MouseMove&)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the cursor leaves an item button
	///
	/// \param func A function callback
	///
	///////////////////////////////////////////////////////////
	void onMouseLeaveItem(const std::function<void(Button*, const E_MouseMove&)>& func);

private:
	void onGainFocus() override;

	void onLoseFocus() override;

private:
	VListView* m_menu;						//!< The menu element
	Uint32 m_selectedItem;					//!< The current selected item index
	float m_itemHeight;						//!< The default item height
	std::vector<Vector4f> m_itemColors;		//!< The default item color
	std::vector<bool> m_customItem;			//!< A list that determines if each is a custom button or not

	std::function<void(Button*, Uint32)> m_onItemAdd;
	std::function<void(Uint32)> m_onItemChange;
	std::function<void(Button*, const E_MouseMove&)> m_onMouseEnterItem;
	std::function<void(Button*, const E_MouseMove&)> m_onMouseLeaveItem;
};

}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Dropdown
/// \ingroup UI
///
/// A dropdown UI element creates a dropdown menu, where each
/// item is a button with different string values. The dropdown
/// menu inherits from button, so it contains all the functionality
/// a standard button has, but the dropdown text is aligned to
/// the left side by default, with a small offset.
///
/// To add items to the dropdown menu, use addItem(). This function
/// takes either a string or a pointer to a button. Adding a button
/// with this function allows for more flexibilty in controlling the
/// button appearance, but most of the time, adding an item in the
/// form of a string will be enough. All item buttons will have a
/// height defined by the user, and the width of each item will be
/// equal to the width of the menu. The item height can be set with
/// setItemHeight(). To set the color of each item, use setItemColor(),
/// or if an alternating color pattern is desired, use setItemColorPattern().
///
/// Custom callback functions can be set for every time a new item is added
/// (another option for customizing the appearance of each item button), when
/// the current selected item is changed, and when the mouse cursor enters
/// and leaves the bounds of an item button.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Make sure to set a default font with Text::setDefaultFont()
///
/// Dropdown dropdown;
/// dropdown.setPosition(50.0f, 50.0f);
/// dropdown.setSize(200.0f, 30.0f);
/// dropdown.setColor(0.2f, 0.2f, 0.25f, 1.0f);
/// dropdown.setItemHeight(28.0f);
/// dropdown.setItemColor(0.18f, 0.18f, 0.22f, 1.0f);
///
/// dropdown.addItem("One");
/// dropdown.addItem("Two");
/// dropdown.addItem("Three");
///
/// // Add it to the UI system
/// ...
///
/// \endcode
///
///////////////////////////////////////////////////////////