#include <poly/UI/ListView.h>
#include <poly/UI/UIParser.h>

namespace poly
{


///////////////////////////////////////////////////////////
void VListView::parse(XmlNode node, const UITemplateMap& templates)
{
	// Parse children nodes first (order shouldn't matter because all transforms should be updated at the end)
	XmlNode childNode = node.getFirstNode();
	while (childNode.exists())
	{
		// Create an element based on the node name
		UIElement* element = 0;
		if (!UIParser::parse(childNode, element) || !element)
		{
			childNode = childNode.getNextSibling();
			continue;
		}

		// Parse the element
		element->parse(childNode, templates);

		// Check if the child has a margin attribute
		XmlAttribute marginsAttr = childNode.getFirstAttribute("margins");
		if (marginsAttr.exists())
		{
			Vector2f margins;
			if (UIParser::parse(marginsAttr, margins))
				addChild(element, margins);
			else
				addChild(element);
		}
		else
			addChild(element);

		childNode = childNode.getNextSibling();
	}

	// Parse as a template first, so that the current node can override the
	// template properties
	XmlAttribute templateAttr = node.getFirstAttribute("template");
	if (templateAttr.exists())
	{
		auto it = templates.find(templateAttr.getValue());
		if (it != templates.end())
		{
			XmlNode templateNode = it->second;
			if (templateNode.exists())
				parse(templateNode, templates);
		}
	}

	// Id
	XmlAttribute idAttr = node.getFirstAttribute("id");
	if (idAttr.exists())
		m_id = idAttr.getValue();
	else
		m_id = "";

	// Position
	XmlAttribute posAttr = node.getFirstAttribute("position");
	if (posAttr.exists())
	{
		Vector2f pos;
		if (UIParser::parse(posAttr, pos))
			setPosition(pos);
	}

	// Rotation
	XmlAttribute rotAttr = node.getFirstAttribute("rotation");
	if (rotAttr.exists())
	{
		float rot;
		if (UIParser::parse(rotAttr, rot))
			setRotation(rot);
	}

	// Size
	XmlAttribute sizeAttr = node.getFirstAttribute("size");
	if (sizeAttr.exists())
	{
		Vector2f size;
		if (UIParser::parse(sizeAttr, size))
			setSize(size);
	}

	// Rel Size
	XmlAttribute relSizeAttr = node.getFirstAttribute("rel_size");
	if (relSizeAttr.exists())
	{
		Vector2f size;
		if (UIParser::parse(relSizeAttr, size))
			setRelSize(size);
	}

	// Width
	XmlAttribute widthAttr = node.getFirstAttribute("width");
	if (widthAttr.exists())
	{
		float width;
		if (UIParser::parse(widthAttr, width))
			setWidth(width);
	}

	// Height
	XmlAttribute heightAttr = node.getFirstAttribute("height");
	if (heightAttr.exists())
	{
		float height;
		if (UIParser::parse(heightAttr, height))
			setHeight(height);
	}

	// Rel Width
	XmlAttribute relWidthAttr = node.getFirstAttribute("rel_width");
	if (relWidthAttr.exists())
	{
		float relWidth;
		if (UIParser::parse(relWidthAttr, relWidth))
			setRelWidth(relWidth);
	}

	// Rel Height
	XmlAttribute relHeightAttr = node.getFirstAttribute("rel_height");
	if (relHeightAttr.exists())
	{
		float relHeight;
		if (UIParser::parse(relHeightAttr, relHeight))
			setRelHeight(relHeight);
	}

	// Origin
	XmlAttribute originAttr = node.getFirstAttribute("origin");
	if (originAttr.exists())
	{
		Vector2f origin;
		UIPosition pos;

		if (UIParser::parse(originAttr, origin))
			setOrigin(origin);

		else if (UIParser::parse(originAttr, pos))
			setOrigin(pos);
	}

	// Anchor
	XmlAttribute anchorAttr = node.getFirstAttribute("anchor");
	if (anchorAttr.exists())
	{
		Vector2f anchor;
		UIPosition pos;

		if (UIParser::parse(anchorAttr, anchor))
			setAnchor(anchor);

		else if (UIParser::parse(anchorAttr, pos))
			setAnchor(pos);
	}

	// Color
	XmlAttribute colorAttr = node.getFirstAttribute("color");
	if (colorAttr.exists())
	{
		Vector4f color;
		if (UIParser::parseColor(colorAttr, color))
			setColor(color);
	}

	// Texture
	XmlAttribute textureAttr = node.getFirstAttribute("texture");
	if (textureAttr.exists())
	{
		Texture* texture;
		if (UIParser::parse(textureAttr, texture))
			setTexture(texture);
	}

	// Texture Rect
	XmlAttribute textureRectAttr = node.getFirstAttribute("texture_rect");
	if (textureRectAttr.exists())
	{
		Vector4f textureRect;
		if (UIParser::parse(textureRectAttr, textureRect))
			setTextureRect(textureRect);
	}

	// Shader
	XmlNode shaderNode = node.getFirstNode("shader");
	if (shaderNode.exists())
	{
		Shader* shader;
		if (UIParser::parse(shaderNode, shader))
			setShader(shader);
	}

	// Visibility
	XmlAttribute visibleAttr = node.getFirstAttribute("visible");
	if (visibleAttr.exists())
	{
		bool visible;
		if (UIParser::parse(visibleAttr, visible))
			setVisible(visible);
	}

	// Flip UV coords
	XmlAttribute flipUvAttr = node.getFirstAttribute("flipped_uv");
	if (flipUvAttr.exists())
	{
		bool flip;
		if (UIParser::parse(flipUvAttr, flip))
			setFlippedUv(flip);
	}

	// Transparent texture
	XmlAttribute transparentAttr = node.getFirstAttribute("transparent");
	if (transparentAttr.exists())
	{
		bool transparent;
		if (UIParser::parse(transparentAttr, transparent))
			setTransparent(transparent);
	}
}


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
void HListView::parse(XmlNode node, const UITemplateMap& templates)
{
	// Parse children nodes first (order shouldn't matter because all transforms should be updated at the end)
	XmlNode childNode = node.getFirstNode();
	while (childNode.exists())
	{
		// Create an element based on the node name
		UIElement* element = 0;
		if (!UIParser::parse(childNode, element) || !element)
		{
			childNode = childNode.getNextSibling();
			continue;
		}

		// Parse the element
		element->parse(childNode, templates);

		// Check if the child has a margin attribute
		XmlAttribute marginsAttr = childNode.getFirstAttribute("margins");
		if (marginsAttr.exists())
		{
			Vector2f margins;
			if (UIParser::parse(marginsAttr, margins))
				addChild(element, margins);
			else
				addChild(element);
		}
		else
			addChild(element);

		childNode = childNode.getNextSibling();
	}

	// Parse as a template first, so that the current node can override the
	// template properties
	XmlAttribute templateAttr = node.getFirstAttribute("template");
	if (templateAttr.exists())
	{
		auto it = templates.find(templateAttr.getValue());
		if (it != templates.end())
		{
			XmlNode templateNode = it->second;
			if (templateNode.exists())
				parse(templateNode, templates);
		}
	}

	// Id
	XmlAttribute idAttr = node.getFirstAttribute("id");
	if (idAttr.exists())
		m_id = idAttr.getValue();
	else
		m_id = "";

	// Position
	XmlAttribute posAttr = node.getFirstAttribute("position");
	if (posAttr.exists())
	{
		Vector2f pos;
		if (UIParser::parse(posAttr, pos))
			setPosition(pos);
	}

	// Rotation
	XmlAttribute rotAttr = node.getFirstAttribute("rotation");
	if (rotAttr.exists())
	{
		float rot;
		if (UIParser::parse(rotAttr, rot))
			setRotation(rot);
	}

	// Size
	XmlAttribute sizeAttr = node.getFirstAttribute("size");
	if (sizeAttr.exists())
	{
		Vector2f size;
		if (UIParser::parse(sizeAttr, size))
			setSize(size);
	}

	// Rel Size
	XmlAttribute relSizeAttr = node.getFirstAttribute("rel_size");
	if (relSizeAttr.exists())
	{
		Vector2f size;
		if (UIParser::parse(relSizeAttr, size))
			setRelSize(size);
	}

	// Width
	XmlAttribute widthAttr = node.getFirstAttribute("width");
	if (widthAttr.exists())
	{
		float width;
		if (UIParser::parse(widthAttr, width))
			setWidth(width);
	}

	// Height
	XmlAttribute heightAttr = node.getFirstAttribute("height");
	if (heightAttr.exists())
	{
		float height;
		if (UIParser::parse(heightAttr, height))
			setHeight(height);
	}

	// Rel Width
	XmlAttribute relWidthAttr = node.getFirstAttribute("rel_width");
	if (relWidthAttr.exists())
	{
		float relWidth;
		if (UIParser::parse(relWidthAttr, relWidth))
			setRelWidth(relWidth);
	}

	// Rel Height
	XmlAttribute relHeightAttr = node.getFirstAttribute("rel_height");
	if (relHeightAttr.exists())
	{
		float relHeight;
		if (UIParser::parse(relHeightAttr, relHeight))
			setRelHeight(relHeight);
	}

	// Origin
	XmlAttribute originAttr = node.getFirstAttribute("origin");
	if (originAttr.exists())
	{
		Vector2f origin;
		UIPosition pos;

		if (UIParser::parse(originAttr, origin))
			setOrigin(origin);

		else if (UIParser::parse(originAttr, pos))
			setOrigin(pos);
	}

	// Anchor
	XmlAttribute anchorAttr = node.getFirstAttribute("anchor");
	if (anchorAttr.exists())
	{
		Vector2f anchor;
		UIPosition pos;

		if (UIParser::parse(anchorAttr, anchor))
			setAnchor(anchor);

		else if (UIParser::parse(anchorAttr, pos))
			setAnchor(pos);
	}

	// Color
	XmlAttribute colorAttr = node.getFirstAttribute("color");
	if (colorAttr.exists())
	{
		Vector4f color;
		if (UIParser::parseColor(colorAttr, color))
			setColor(color);
	}

	// Texture
	XmlAttribute textureAttr = node.getFirstAttribute("texture");
	if (textureAttr.exists())
	{
		Texture* texture;
		if (UIParser::parse(textureAttr, texture))
			setTexture(texture);
	}

	// Texture Rect
	XmlAttribute textureRectAttr = node.getFirstAttribute("texture_rect");
	if (textureRectAttr.exists())
	{
		Vector4f textureRect;
		if (UIParser::parse(textureRectAttr, textureRect))
			setTextureRect(textureRect);
	}

	// Shader
	XmlNode shaderNode = node.getFirstNode("shader");
	if (shaderNode.exists())
	{
		Shader* shader;
		if (UIParser::parse(shaderNode, shader))
			setShader(shader);
	}

	// Visibility
	XmlAttribute visibleAttr = node.getFirstAttribute("visible");
	if (visibleAttr.exists())
	{
		bool visible;
		if (UIParser::parse(visibleAttr, visible))
			setVisible(visible);
	}

	// Flip UV coords
	XmlAttribute flipUvAttr = node.getFirstAttribute("flipped_uv");
	if (flipUvAttr.exists())
	{
		bool flip;
		if (UIParser::parse(flipUvAttr, flip))
			setFlippedUv(flip);
	}

	// Transparent texture
	XmlAttribute transparentAttr = node.getFirstAttribute("transparent");
	if (transparentAttr.exists())
	{
		bool transparent;
		if (UIParser::parse(transparentAttr, transparent))
			setTransparent(transparent);
	}
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