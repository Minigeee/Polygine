#include <poly/Core/ObjectPool.h>

#include <poly/UI/Dropdown.h>
#include <poly/UI/ListView.h>

namespace poly
{


///////////////////////////////////////////////////////////
Dropdown::Dropdown() :
	m_menu			(Pool<VListView>::alloc()),
	m_selectedItem	(0),
	m_itemHeight	(25.0f),
	m_itemColor		(1.0f)
{
	m_menu->setVisible(false);
	m_menu->setAnchor(UIPosition::BotLeft);
	addChild(m_menu);
}


///////////////////////////////////////////////////////////
Dropdown::~Dropdown()
{
	// Free items
	clearItems();

	Pool<VListView>::free(m_menu);
	m_menu = 0;
}


///////////////////////////////////////////////////////////
void Dropdown::addItem(const std::string& name)
{
	Button* item = Pool<Button>::alloc();
	item->setString(name);
	item->setTextAlign(UIPosition::Left);
	item->setTextOffset(5.0f, 0.0f);
	item->setSize(getPixelSize().x, m_itemHeight);
	item->setColor(m_itemColor);
	item->setVisible(false);

	// Get index
	Uint32 index = m_menu->getChildren().size();

	// Set callback functions
	if (m_onMouseEnterItem)
		item->onMouseEnter(std::bind(m_onMouseEnterItem, item, std::placeholders::_1));
	if (m_onMouseLeaveItem)
		item->onMouseLeave(std::bind(m_onMouseLeaveItem, item, std::placeholders::_1));
	item->onPress(std::bind(&Dropdown::setSelectedItem, this, index));

	if (m_onItemAdd)
		m_onItemAdd(item, index);

	m_menu->addChild(item);
}


///////////////////////////////////////////////////////////
void Dropdown::removeItem(Uint32 index)
{
	Button* item = (Button*)m_menu->getChildren()[index];

	// Remove from list
	m_menu->removeChild(item);

	// Free item
	Pool<Button>::free(item);
}


///////////////////////////////////////////////////////////
void Dropdown::clearItems()
{
	const std::vector<UIElement*>& children = m_menu->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
	{
		Button* item = (Button*)children[i];

		// Remove from list
		m_menu->removeChild(item);

		// Free item
		Pool<Button>::free(item);
	}
}


///////////////////////////////////////////////////////////
void Dropdown::setSelectedItem(Uint32 index)
{
	if (index == m_selectedItem) return;

	Button* item = (Button*)m_menu->getChildren()[index];
	m_selectedItem = index;

	// Set selected text
	setString(item->getString());

	// Hide the menu
	m_menu->setVisible(false);

	// Callback
	if (m_onItemChange)
		m_onItemChange(item->getString());
}


///////////////////////////////////////////////////////////
void Dropdown::setItemHeight(float height)
{
	m_itemHeight = height;
}


///////////////////////////////////////////////////////////
void Dropdown::setItemColor(const Vector4f& color)
{
	m_itemColor = color;
}


///////////////////////////////////////////////////////////
void Dropdown::setItemColor(float r, float g, float b, float a)
{
	m_itemColor = Vector4f(r, g, b, a);
}


///////////////////////////////////////////////////////////
Uint32 Dropdown::getSelectedItem() const
{
	return m_selectedItem;
}


///////////////////////////////////////////////////////////
Button* Dropdown::getItem(Uint32 index)
{
	return (Button*)(m_menu->getChildren()[index]);
}


///////////////////////////////////////////////////////////
void Dropdown::onItemAdd(const std::function<void(Button*, Uint32)>& func)
{
	m_onItemAdd = func;

	// Apply to items already in the list
	const std::vector<UIElement*>& children = m_menu->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
	{
		Button* item = (Button*)children[i];
		m_onItemAdd(item, i);
	}
}


///////////////////////////////////////////////////////////
void Dropdown::onItemChange(const std::function<void(const std::string&)>& func)
{
	m_onItemChange = func;
}


///////////////////////////////////////////////////////////
void Dropdown::onMouseEnterItem(const std::function<void(Button*, const E_MouseMove&)>& func)
{
	m_onMouseEnterItem = func;

	const std::vector<UIElement*>& children = m_menu->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
	{
		Button* item = (Button*)children[i];
		item->onMouseEnter(std::bind(m_onMouseEnterItem, item, std::placeholders::_1));
	}
}


///////////////////////////////////////////////////////////
void Dropdown::onMouseLeaveItem(const std::function<void(Button*, const E_MouseMove&)>& func)
{
	m_onMouseLeaveItem = func;

	const std::vector<UIElement*>& children = m_menu->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
	{
		Button* item = (Button*)children[i];
		item->onMouseLeave(std::bind(m_onMouseLeaveItem, item, std::placeholders::_1));
	}
}


///////////////////////////////////////////////////////////
void Dropdown::onGainFocus()
{
	// Bring this item to front
	moveToFront();

	// Show menu
	m_menu->setVisible(true);
}


///////////////////////////////////////////////////////////
void Dropdown::onLoseFocus()
{
	// Show menu
	m_menu->setVisible(false);
}


}