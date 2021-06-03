#include <poly/Core/ObjectPool.h>

#include <poly/UI/Dropdown.h>
#include <poly/UI/ListView.h>
#include <poly/UI/Text.h>
#include <poly/UI/UIParser.h>

namespace poly
{


///////////////////////////////////////////////////////////
Dropdown::Dropdown() :
	m_menu			(Pool<VListView>::alloc()),
	m_selectedItem	(0),
	m_itemHeight	(25.0f),
	m_itemColors	()
{
	m_itemColors.push_back(Vector4f(1.0f));

	m_menu->setVisible(false);
	m_menu->setAnchor(UIPosition::BotLeft);
	addChild(m_menu);

	// Default dropdown settings
	setTextAlign(UIPosition::Left);
	setTextOffset(5.0f, 0.0f);
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
void Dropdown::parse(XmlNode node, const UITemplateMap& templates)
{
	// Default parse
	UIElement::parse(node, templates);

	// Text value
	XmlAttribute valueAttr = node.getFirstAttribute("value");
	if (valueAttr.exists())
		setString(valueAttr.getValue());
	else
	{
		XmlAttribute textAttr = node.getFirstAttribute("text");
		if (textAttr.exists())
			setString(textAttr.getValue());
	}

	// Text align
	XmlAttribute textAlignAttr = node.getFirstAttribute("text_align");
	if (textAlignAttr.exists())
	{
		UIPosition align;
		if (UIParser::parse(textAlignAttr, align))
			setTextAlign(align);
	}

	// Text offset
	XmlAttribute textOffsetAttr = node.getFirstAttribute("text_offset");
	if (textOffsetAttr.exists())
	{
		Vector2f offset;
		if (UIParser::parse(textOffsetAttr, offset))
			setTextOffset(offset);
	}

	// Item height
	XmlAttribute itemHeightAttr = node.getFirstAttribute("item_height");
	if (itemHeightAttr.exists())
	{
		float height;
		if (UIParser::parse(itemHeightAttr, height))
			setItemHeight(height);
	}

	// Item color
	XmlAttribute itemColorAttr = node.getFirstAttribute("item_color");
	if (itemColorAttr.exists())
	{
		Vector4f color;
		if (UIParser::parseColor(itemColorAttr, color))
			setItemColor(color);
	}

	// On item add
	XmlAttribute onItemAddAttr = node.getFirstAttribute("on_item_add");
	if (onItemAddAttr.exists())
	{
		std::function<void(Button*, Uint32)> func;
		if (UIParser::parse(onItemAddAttr, this, func))
			onItemAdd(func);
	};

	// On item add
	XmlAttribute onItemChangeAttr = node.getFirstAttribute("on_item_change");
	if (onItemChangeAttr.exists())
	{
		std::function<void(Uint32)> func;
		if (UIParser::parse(onItemChangeAttr, this, func))
			onItemChange(func);
	};

	// On item add
	XmlAttribute onMouseEnterItemAttr = node.getFirstAttribute("on_mouse_enter_item");
	if (onMouseEnterItemAttr.exists())
	{
		std::function<void(Button*, const E_MouseMove&)> func;
		if (UIParser::parse(onMouseEnterItemAttr, this, func))
			onMouseEnterItem(func);
	};

	// On item add
	XmlAttribute onMouseLeaveItemAttr = node.getFirstAttribute("on_mouse_leave_item");
	if (onMouseLeaveItemAttr.exists())
	{
		std::function<void(Button*, const E_MouseMove&)> func;
		if (UIParser::parse(onMouseLeaveItemAttr, this, func))
			onMouseLeaveItem(func);
	};

	// Parse text options
	XmlNode textNode = node.getFirstNode("dropdown_text");
	if (textNode.exists())
	{
		getText()->parse(textNode, templates);

		// Reset the text id because it shouldn't have an id
		getText()->setId("");
	}

	// Add dropdown items
	XmlNode itemNode = node.getFirstNode("dropdown_item");
	while (itemNode.exists())
	{
		if (strlen(itemNode.getValue()))
			addItem(itemNode.getValue());

		else
		{
			Button* item = Pool<Button>::alloc();
			item->parse(itemNode, templates);

			// Reset the item id because it shouldn't have an id
			item->setId("");

			// Add the custom button
			addItem(item);
		}

		itemNode = itemNode.getNextSibling("dropdown_item");
	}
}


///////////////////////////////////////////////////////////
void Dropdown::addItem(const std::string& name)
{
	// If this is the first item, set value
	if (!m_menu->getChildren().size())
		setString(name);

	// Get index
	Uint32 index = m_menu->getChildren().size();

	Uint32 numColors = m_itemColors.size();

	Button* item = Pool<Button>::alloc();
	item->setString(name);
	item->setTextAlign(UIPosition::Left);
	item->setTextOffset(5.0f, 0.0f);
	item->setSize(getPixelSize().x, m_itemHeight);
	item->setColor(m_itemColors[index % numColors]);
	item->setVisible(false);

	// Set callback functions
	if (m_onMouseEnterItem)
		item->onMouseEnter(std::bind(m_onMouseEnterItem, item, std::placeholders::_1));
	if (m_onMouseLeaveItem)
		item->onMouseLeave(std::bind(m_onMouseLeaveItem, item, std::placeholders::_1));
	item->onPress(std::bind(&Dropdown::setSelectedItem, this, index));

	if (m_onItemAdd)
		m_onItemAdd(item, index);

	m_menu->addChild(item);
	m_customItem.push_back(false);
}


///////////////////////////////////////////////////////////
void Dropdown::addItem(Button* item)
{
	// If this is the first item, set value
	if (!m_menu->getChildren().size())
		setString(item->getString());

	item->setTextAlign(UIPosition::Left);
	item->setTextOffset(5.0f, 0.0f);
	item->setSize(getPixelSize().x, m_itemHeight);
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
	m_customItem.push_back(true);
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
	if (!Pool<VListView>::isInitialized()) return;

	const std::vector<UIElement*>& children = m_menu->getChildren();
	for (int i = children.size() - 1; i >= 0; --i)
	{
		Button* item = (Button*)children[i];

		// Remove from list
		m_menu->removeChild(item);

		// Free item if it is not a custom item
		if (!m_customItem[i])
			Pool<Button>::free(item);
	}

	m_customItem.clear();
}


///////////////////////////////////////////////////////////
void Dropdown::setItems(const std::vector<std::string>& items)
{
	// Clear items
	clearItems();

	// Add each item in the list
	for (Uint32 i = 0; i < items.size(); ++i)
		addItem(items[i]);
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
		m_onItemChange(index);
}


///////////////////////////////////////////////////////////
void Dropdown::setItemHeight(float height)
{
	m_itemHeight = height;
}


///////////////////////////////////////////////////////////
void Dropdown::setItemColor(const Vector4f& color)
{
	m_itemColors[0] = color;
}


///////////////////////////////////////////////////////////
void Dropdown::setItemColor(float r, float g, float b, float a)
{
	m_itemColors[0] = Vector4f(r, g, b, a);
}


///////////////////////////////////////////////////////////
void Dropdown::setItemColorPattern(const std::vector<Vector4f>& colors)
{
	m_itemColors = colors;

	Uint32 numColors = m_itemColors.size();

	// Apply the pattern to current items
	for (Uint32 i = 0; i < m_children.size(); ++i)
		m_children[i]->setColor(m_itemColors[i % numColors]);
}


///////////////////////////////////////////////////////////
void Dropdown::setVisible(bool visible, bool recursive)
{
	bool origVal = m_menu->isVisible();

	// Default function
	UIElement::setVisible(visible, recursive);

	// Set text cursor to its original state
	m_menu->setVisible(origVal);
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
void Dropdown::onItemChange(const std::function<void(Uint32)>& func)
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