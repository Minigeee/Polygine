#include <poly/Core/ObjectPool.h>

#include <poly/Math/Functions.h>

#include <poly/UI/ScrollView.h>
#include <poly/UI/Slider.h>
#include <poly/UI/UIParser.h>

namespace poly
{


///////////////////////////////////////////////////////////
ScrollBody::ScrollBody() :
	m_clipMargin	(5.0f)
{

}


///////////////////////////////////////////////////////////
void ScrollBody::setClipMargin(float margin)
{
	m_clipMargin = margin;
}


///////////////////////////////////////////////////////////
float ScrollBody::getClipMargin() const
{
	return m_clipMargin;
}


///////////////////////////////////////////////////////////
Vector4f ScrollBody::getClipRect()
{
	return Vector4f(m_parent->getAbsPosition() + m_clipMargin, m_parent->getPixelSize() - 2.0f * m_clipMargin);
}


///////////////////////////////////////////////////////////
ScrollView::ScrollView() :
	m_scrollBody	(Pool<ScrollBody>::alloc()),
	m_scrollBar		(Pool<Slider>::alloc()),
	m_scrollSpeed	(50.0f),
	m_minY			(0.0f),
	m_maxY			(0.0f)
{
	// Setup scroll body
	UIElement::addChild(m_scrollBody);

	m_scrollBody->setSize(0.0f, 0.0f);
	m_scrollBody->setVisible(false, false);

	// Setup scroll bar
	UIElement::addChild(m_scrollBar);

	m_scrollBar->setOrigin(UIPosition::BotLeft);
	m_scrollBar->setAnchor(UIPosition::TopRight);
	m_scrollBar->setRotation(-90.0f);
	m_scrollBar->setHeight(10.0f);
	m_scrollBar->onValueChange(std::bind(&ScrollView::onScrollBarChange, this, std::placeholders::_1));

	// When there is nothing in the scroll body, hide the scroll bar
	m_scrollBar->setVisible(false);
}


///////////////////////////////////////////////////////////
ScrollView::~ScrollView()
{
	Pool<ScrollBody>::free(m_scrollBody);
	Pool<Slider>::free(m_scrollBar);
	m_scrollBody = 0;
	m_scrollBar = 0;
}


///////////////////////////////////////////////////////////
void ScrollView::parse(XmlNode node)
{
	// Default parse
	UIElement::parse(node);

	// Clip margins
	XmlAttribute marginsAttr = node.getFirstAttribute("clip_margins");
	if (marginsAttr.exists())
	{
		float margins;
		if (UIParser::parse(marginsAttr, margins))
			setClipMargin(margins);
	}

	// Scroll speed
	XmlAttribute speedAttr = node.getFirstAttribute("scroll_speed");
	if (speedAttr.exists())
	{
		float speed;
		if (UIParser::parse(speedAttr, speed))
			setScrollSpeed(speed);
	}

	// Scroll bar width
	XmlAttribute barWidthAttr = node.getFirstAttribute("scroll_bar_width");
	if (barWidthAttr.exists())
	{
		float barWidth;
		if (UIParser::parse(barWidthAttr, barWidth))
			setScrollBarWidth(barWidth);
	}

	// Slider
	XmlNode sliderNode = node.getFirstNode("scroll_bar");
	if (sliderNode.exists())
		m_scrollBar->parse(sliderNode);
}


///////////////////////////////////////////////////////////
void ScrollView::addChild(UIElement* child)
{
	const Vector2f& pos = child->getRelPosition();
	const Vector2f& size = child->getPixelSize();

	// Update bounds
	Vector2f points[] =
	{
		pos,
		pos + Vector2f(size.x, 0.0f),
		pos + Vector2f(0.0f, size.y),
		pos + size,
	};

	// Adjust for rotation
	float angle = -rad(m_absRotation);
	float ca = cos(angle);
	float sa = sin(angle);

	// Transform all points and keep track of min and max y
	float minY = pos.y;
	float maxY = pos.y;
	for (Uint32 i = 0; i < 4; ++i)
	{
		Vector2f p = points[i];

		// Apply rotation
		p = Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca);

		if (p.y < minY)
			minY = p.y;
		if (p.y > maxY)
			maxY = p.y;
	}

	if (minY < m_minY)
		m_minY = minY;
	if (maxY > m_maxY)
		m_maxY = maxY;

	// Update scroll bar size
	float bodyHeight = getPixelSize().y;
	float width = bodyHeight * bodyHeight / (m_maxY - m_minY);

	// Set scroll bar and change visibility
	if (width < bodyHeight)
	{
		if (!m_scrollBar->isVisible())
			m_scrollBar->setVisible(true);

		m_scrollBar->getSliderButton()->setWidth(width);
	}
	else if (m_scrollBar->isVisible())
		m_scrollBar->setVisible(false);

	m_scrollBody->addChild(child);
}


///////////////////////////////////////////////////////////
void ScrollView::removeChild(UIElement* child)
{
	m_scrollBody->removeChild(child);
}


///////////////////////////////////////////////////////////
void ScrollView::updateViewBounds()
{
	const std::vector<UIElement*>& children = m_scrollBody->getChildren();

	// Reset bounds
	m_minY = m_maxY = 0.0f;

	for (Uint32 i = 0; i < children.size(); ++i)
	{
		UIElement* child = children[i];
		const Vector2f& pos = child->getRelPosition();
		const Vector2f& size = child->getPixelSize();

		// Update bounds
		Vector2f points[] =
		{
			pos,
			pos + Vector2f(size.x, 0.0f),
			pos + Vector2f(0.0f, size.y),
			pos + size,
		};

		// Adjust for rotation
		float angle = -rad(m_absRotation);
		float ca = cos(angle);
		float sa = sin(angle);

		// Transform all points and keep track of min and max y
		float minY = pos.y;
		float maxY = pos.y;
		for (Uint32 i = 0; i < 4; ++i)
		{
			Vector2f p = points[i];

			// Apply rotation
			p = Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca);

			if (p.y < minY)
				minY = p.y;
			if (p.y > maxY)
				maxY = p.y;
		}

		if (minY < m_minY)
			m_minY = minY;
		if (maxY > m_maxY)
			m_maxY = maxY;
	}
}


///////////////////////////////////////////////////////////
void ScrollView::setClipMargin(float margin)
{
	m_scrollBody->setClipMargin(margin);
}


///////////////////////////////////////////////////////////
void ScrollView::setScrollOffset(float offset)
{
	m_scrollBody->setPosition(0.0f, -offset);
}


///////////////////////////////////////////////////////////
void ScrollView::setScrollSpeed(float speed)
{
	m_scrollSpeed = speed;
}


///////////////////////////////////////////////////////////
void ScrollView::setScrollBarWidth(float size)
{
	m_scrollBar->setHeight(size);
}


///////////////////////////////////////////////////////////
float ScrollView::getClipMargin() const
{
	return m_scrollBody->getClipMargin();
}


///////////////////////////////////////////////////////////
float ScrollView::getScrollOffset() const
{
	return -m_scrollBody->getRelPosition().y;
}


///////////////////////////////////////////////////////////
float ScrollView::getScrollSpeed() const
{
	return m_scrollSpeed;
}


///////////////////////////////////////////////////////////
float ScrollView::getScrollBarWidth() const
{
	return m_scrollBar->getPixelSize().y;
}


///////////////////////////////////////////////////////////
Slider* ScrollView::getScrollBar()
{
	return m_scrollBar;
}


///////////////////////////////////////////////////////////
void ScrollView::onMouseScroll(const E_MouseScroll& e)
{
	// Change scroll bar value
	float value =
		m_scrollBar->getValue() - e.m_dy * m_scrollSpeed /
		(m_maxY - m_minY - getPixelSize().y + 2.0f * m_scrollBody->getClipMargin());
	m_scrollBar->setValue(value);
}


///////////////////////////////////////////////////////////
void ScrollView::onScrollBarChange(float value)
{
	// Set position
	float y = -(m_maxY - m_minY - getPixelSize().y + 2.0f * m_scrollBody->getClipMargin()) * value;
	y += m_scrollBody->getClipMargin();
	m_scrollBody->setPosition(0.0f, y);
}


///////////////////////////////////////////////////////////
void ScrollView::updateTransforms()
{
	if (m_transformChanged)
	{
		UIElement::updateTransforms();

		// Update scroll bar height
		m_scrollBar->setWidth(m_pixelSize.y);
	}
}


///////////////////////////////////////////////////////////
bool ScrollView::handlesMouseEvents() const
{
	return true;
}


}