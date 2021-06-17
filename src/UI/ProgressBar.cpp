#include <poly/Core/ObjectPool.h>

#include <poly/UI/ProgressBar.h>
#include <poly/UI/UIParser.h>

namespace poly
{


///////////////////////////////////////////////////////////
ProgressBar::ProgressBar() :
	m_fillBox		(Pool<UIElement>::alloc()),
	m_padding		(0.0f),
	m_value			(0.0f)
{
	addChild(m_fillBox);
}


///////////////////////////////////////////////////////////
ProgressBar::~ProgressBar()
{
	if (m_fillBox)
		Pool<UIElement>::free(m_fillBox);
	m_fillBox = 0;
}


///////////////////////////////////////////////////////////
void ProgressBar::parse(XmlNode node, const UITemplateMap& templates)
{
	UIElement::parse(node, templates);

	// Padding
	XmlAttribute paddingAttr = node.getFirstAttribute("padding");
	if (paddingAttr.exists())
	{
		Vector2f padding;
		if (UIParser::parse(paddingAttr, padding))
			setPadding(padding);
	}

	// Fill color
	XmlAttribute fillColorAttr = node.getFirstAttribute("fill_color");
	if (fillColorAttr.exists())
	{
		Vector4f color;
		if (UIParser::parseColor(fillColorAttr, color))
			setFillColor(color);
	}

	// Fill texture
	XmlAttribute fillTextureAttr = node.getFirstAttribute("fill_texture");
	if (fillTextureAttr.exists())
	{
		Texture* texture;
		if (UIParser::parse(fillTextureAttr, texture))
			setFillTexture(texture);
	}

	// Value
	XmlAttribute valueAttr = node.getFirstAttribute("value");
	if (valueAttr.exists())
	{
		float value;
		if (UIParser::parse(valueAttr, value))
			setValue(value);
	}

	// Parse fill box options
	XmlNode boxNode = node.getFirstNode("fill_box");
	if (boxNode.exists())
	{
		m_fillBox->parse(boxNode, templates);

		// Reset the box id because it shouldn't have an id
		m_fillBox->setId("");
	}
}


///////////////////////////////////////////////////////////
void ProgressBar::setPadding(const Vector2f& padding)
{
	m_padding = padding;
	updateFillBox();
}


///////////////////////////////////////////////////////////
void ProgressBar::setPadding(float x, float y)
{
	m_padding = Vector2f(x, y);
	updateFillBox();
}


///////////////////////////////////////////////////////////
void ProgressBar::setFillColor(const Vector4f& color)
{
	m_fillBox->setColor(color);
}


///////////////////////////////////////////////////////////
void ProgressBar::setFillColor(float r, float g, float b, float a)
{
	m_fillBox->setColor(r, g, b, a);
}


///////////////////////////////////////////////////////////
void ProgressBar::setFillTexture(Texture* texture)
{
	m_fillBox->setTexture(texture);
}


///////////////////////////////////////////////////////////
void ProgressBar::setValue(float value)
{
	if (value != m_value)
	{
		m_value = value;

		if (m_value > 1.0f)
			m_value = 1.0f;
		else if (m_value < 0.0f)
			m_value = 0.0f;

		updateFillBox();
	}
}


///////////////////////////////////////////////////////////
const Vector2f& ProgressBar::getPadding() const
{
	return m_padding;
}


///////////////////////////////////////////////////////////
const Vector4f& ProgressBar::getFillColor() const
{
	return m_fillBox->getColor();
}


///////////////////////////////////////////////////////////
Texture* ProgressBar::getFillTexture() const
{
	return m_fillBox->getTexture();
}


///////////////////////////////////////////////////////////
float ProgressBar::getValue() const
{
	return m_value;
}


///////////////////////////////////////////////////////////
UIElement* ProgressBar::getFillBox() const
{
	return m_fillBox;
}


///////////////////////////////////////////////////////////
void ProgressBar::updateFillBox()
{
	m_fillBox->setOrigin(UIPosition::TopLeft);
	m_fillBox->setAnchor(UIPosition::TopLeft);

	// Position
	m_fillBox->setPosition(m_padding);

	// Size
	Vector2f maxSize = getPixelSize() - 2.0f * m_padding;
	m_fillBox->setSize(maxSize.x * m_value, maxSize.y);
}


}