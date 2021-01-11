#include <poly/UI/ListView.h>

namespace poly
{


///////////////////////////////////////////////////////////
void VListView::addChild(UIElement* child)
{
	// Normal add
	UIElement::addChild(child);

	const Vector2f& size = child->getPixelSize();
	m_useRelSize = Vector2b(false);

	// Calculate position
	child->setPosition(child->getRelPosition().x, m_pixelSize.y + child->getOrigin().y * size.y);

	// Calculate size
	float minX = child->getRelPosition().x - child->getOrigin().x * size.x;
	float maxX = child->getRelPosition().x + (1.0f - child->getOrigin().x) * size.x;
	if (maxX - minX > m_pixelSize.x)
		m_pixelSize.x = maxX - minX;

	m_pixelSize.y += size.y;

	// Keep track of margins
	m_margins.push_back(Vector2f(0.0f));
}


///////////////////////////////////////////////////////////
void VListView::addChild(UIElement* child, const Vector2f& margins)
{
	// Normal add
	UIElement::addChild(child);

	const Vector2f& size = child->getPixelSize();
	m_useRelSize = Vector2b(false);

	// Calculate position
	child->setPosition(child->getRelPosition().x, m_pixelSize.y + child->getOrigin().y * size.y + margins.x);

	// Calculate size
	float minX = child->getRelPosition().x - child->getOrigin().x * size.x;
	float maxX = child->getRelPosition().x + (1.0f - child->getOrigin().x) * size.x;
	if (maxX - minX > m_pixelSize.x)
		m_pixelSize.x = maxX - minX;

	m_pixelSize.y += size.y + margins.x + margins.y;

	// Keep track of margins
	m_margins.push_back(margins);
}


///////////////////////////////////////////////////////////
void VListView::removeChild(UIElement* child)
{
	// Keep index
	Uint32 index = child->getIndex();

	// Standard remove
	UIElement::removeChild(child);

	// Change size
	const Vector2f& margins = m_margins[index];
	m_pixelSize.y -= child->getPixelSize().y + margins.x + margins.y;

	// Remove margins
	m_margins.erase(m_margins.begin() + index);
}


///////////////////////////////////////////////////////////
void VListView::getQuads(std::vector<UIQuad>& quads)
{
	// Do nothing
}


///////////////////////////////////////////////////////////
void HListView::addChild(UIElement* child)
{
	// Normal add
	UIElement::addChild(child);

	const Vector2f& size = child->getPixelSize();
	m_useRelSize = Vector2b(false);

	// Calculate position
	child->setPosition(m_pixelSize.x + child->getOrigin().x * size.x, child->getRelPosition().y);

	// Calculate size
	float minY = child->getRelPosition().y - child->getOrigin().y * size.y;
	float maxY = child->getRelPosition().y + (1.0f - child->getOrigin().y) * size.y;
	if (maxY - minY > m_pixelSize.y)
		m_pixelSize.y = maxY - minY;

	m_pixelSize.x += size.x;

	// Keep track of margins
	m_margins.push_back(Vector2f(0.0f));
}


///////////////////////////////////////////////////////////
void HListView::addChild(UIElement* child, const Vector2f& margins)
{
	// Normal add
	UIElement::addChild(child);

	const Vector2f& size = child->getPixelSize();
	m_useRelSize = Vector2b(false);

	// Calculate position
	child->setPosition(m_pixelSize.x + child->getOrigin().x * size.x + margins.x, child->getRelPosition().y);

	// Calculate size
	float minY = child->getRelPosition().y - child->getOrigin().y * size.y;
	float maxY = child->getRelPosition().y + (1.0f - child->getOrigin().y) * size.y;
	if (maxY - minY > m_pixelSize.y)
		m_pixelSize.y = maxY - minY;

	m_pixelSize.x += size.x + margins.x + margins.y;

	// Keep track of margins
	m_margins.push_back(margins);
}


///////////////////////////////////////////////////////////
void HListView::removeChild(UIElement* child)
{
	// Keep index
	Uint32 index = child->getIndex();

	// Standard remove
	UIElement::removeChild(child);

	// Change size
	const Vector2f& margins = m_margins[index];
	m_pixelSize.x -= child->getPixelSize().x + margins.x + margins.y;

	// Remove margins
	m_margins.erase(m_margins.begin() + index);
}


///////////////////////////////////////////////////////////
void HListView::getQuads(std::vector<UIQuad>& quads)
{
	// Do nothing
}


}