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
	/// \brief Add a dropdown menu item
	///
	/// When adding an item, a new button is created and all its
	/// handlers will automatically be connected. It is not possible
	/// to customize each item button much besides changing its height
	/// and color, but onItemAdd() can be used for more customization
	/// by giving access to the item button.
	///
	///////////////////////////////////////////////////////////
	void addItem(const std::string& name);

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
	VListView* m_menu;			//!< The menu element
	Uint32 m_selectedItem;		//!< The current selected item index
	float m_itemHeight;			//!< The default item height
	Vector4f m_itemColor;		//!< The default item color

	std::function<void(Button*, Uint32)> m_onItemAdd;
	std::function<void(Uint32)> m_onItemChange;
	std::function<void(Button*, const E_MouseMove&)> m_onMouseEnterItem;
	std::function<void(Button*, const E_MouseMove&)> m_onMouseLeaveItem;
};

}

#endif