
#include <poly/Math/Functions.h>

#include <poly/UI/UIElement.h>
#include <poly/UI/UIParser.h>

namespace poly
{

namespace priv
{

///////////////////////////////////////////////////////////
Vector2f getPositionFromEnum(UIPosition pos)
{
	switch (pos)
	{
	case UIPosition::TopLeft:
		return Vector2f(0.0f, 0.0f);
	case UIPosition::TopCenter:
		return Vector2f(0.5f, 0.0f);
	case UIPosition::TopRight:
		return Vector2f(1.0f, 0.0f);
	case UIPosition::Left:
		return Vector2f(0.0f, 0.5f);
	case UIPosition::Center:
		return Vector2f(0.5f, 0.5f);
	case UIPosition::Right:
		return Vector2f(1.0f, 0.5f);
	case UIPosition::BotLeft:
		return Vector2f(0.0f, 1.0f);
	case UIPosition::BotCenter:
		return Vector2f(0.5f, 1.0f);
	case UIPosition::BotRight:
		return Vector2f(1.0f, 1.0f);
	default:
		return Vector2f(0.0f, 0.0f);
	}
}

}


///////////////////////////////////////////////////////////
Shader UIElement::s_shader;


///////////////////////////////////////////////////////////
UIElement::UIElement() :
	m_parent				(0),
	m_id					(""),
	m_relPosition			(0.0f),
	m_absPosition			(0.0f),
	m_relRotation			(0.0f),
	m_absRotation			(0.0f),
	m_relSize				(0.0f),
	m_pixelSize				(0.0f),
	m_useRelSize			(false),
	m_origin				(0.0f),
	m_anchor				(0.0f),
	m_color					(1.0f),
	m_texture				(0),
	m_textureRect			(0.0f, 0.0f, 1.0f, 1.0f),
	m_srcBlend				(BlendFactor::SrcAlpha),
	m_dstBlend				(BlendFactor::OneMinusSrcAlpha),
	m_shader				(getDefaultShader()),
	m_hasFlippedUv			(false),
	m_isVisible				(true),
	m_index					(0),
	m_transformChanged		(false),
	m_isColorTransparent	(false),
	m_isTextureTransparent	(false),
	m_hasHover				(false),
	m_hasFocus				(false)
{

}


///////////////////////////////////////////////////////////
void UIElement::parse(XmlNode node, const UITemplateMap& templates)
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

		// Add the element as a child
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
}


///////////////////////////////////////////////////////////
void UIElement::update(float dt)
{

}


///////////////////////////////////////////////////////////
void UIElement::addChild(UIElement* child)
{
	// Add to children list
	m_children.push_back(child);
	
	child->m_parent = this;
	child->m_index = m_children.size() - 1;
}


///////////////////////////////////////////////////////////
void UIElement::removeChild(UIElement* child)
{
	// Only remove if this element is a child
	if (child->m_index < m_children.size()) return;
	if (m_children[child->m_index] != child) return;

	// Shift all elements above the target element down
	for (Uint32 i = child->m_index; i < m_children.size() - 1; ++i)
	{
		m_children[i] = m_children[i + 1];
		--m_children[i]->m_index;
	}

	// Pop from end of the list
	m_children.pop_back();
}


///////////////////////////////////////////////////////////
void UIElement::setId(const std::string& id)
{
	m_id = id;
}


///////////////////////////////////////////////////////////
void UIElement::moveToFront()
{
	// Move to back of list
	if (m_parent)
		setIndex(m_parent->m_children.size() - 1);
}


///////////////////////////////////////////////////////////
void UIElement::moveToBack()
{
	// Move to front of list
	setIndex(0);
}


///////////////////////////////////////////////////////////
void UIElement::setIndex(Uint32 index)
{
	// Can't change index if this is the root element
	if (!m_parent || index >= m_parent->m_children.size()) return;

	if (index > m_index)
	{
		for (Uint32 i = m_index; i < index; ++i)
		{
			m_parent->m_children[i] = m_parent->m_children[i + 1];
			--m_parent->m_children[i]->m_index;
		}
	}
	else
	{
		for (int i = m_index; i > (int)index; --i)
		{
			m_parent->m_children[i] = m_parent->m_children[i - 1];
			++m_parent->m_children[i]->m_index;
		}
	}

	// Insert element into correct spot
	m_parent->m_children[index] = this;
	m_index = index;
}


///////////////////////////////////////////////////////////
void UIElement::markTransformDirty()
{
	m_transformChanged = true;

	for (Uint32 i = 0; i < m_children.size(); ++i)
		m_children[i]->markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::updateTransforms()
{
	if (m_transformChanged)
	{
		m_absPosition = m_relPosition;
		m_absRotation = m_relRotation;

		if (m_parent)
		{
			// Update parent transforms
			m_parent->updateTransforms();

			Vector2f parentOrigin = m_parent->m_origin * m_parent->m_pixelSize;
			Vector2f anchor = m_anchor * m_parent->m_pixelSize;

			// Calculate rotations for transforming element position
			float angle = -rad(m_parent->m_absRotation);
			float c = cos(angle);
			float s = sin(angle);

			Vector2f& p = m_absPosition;
			p = p - parentOrigin + anchor;
			p = Vector2f(p.x * c - p.y * s, p.x * s + p.y * c);
			p += m_parent->m_absPosition;
			m_absRotation += m_parent->m_absRotation;

			// Update sizes
			if (m_useRelSize.x)
				m_pixelSize.x = m_relSize.x * m_parent->m_pixelSize.x;
			else
				m_relSize.x = m_pixelSize.x / m_parent->m_pixelSize.x;

			if (m_useRelSize.y)
				m_pixelSize.y = m_relSize.y * m_parent->m_pixelSize.y;
			else
				m_relSize.y = m_pixelSize.y / m_parent->m_pixelSize.y;
		}

		// Keep rotation within range
		m_absRotation = fmodf(m_absRotation, 360.0f);

		m_transformChanged = false;
	}
}


///////////////////////////////////////////////////////////
void UIElement::setPosition(const Vector2f& pos)
{
	m_relPosition = pos;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setPosition(float x, float y)
{
	m_relPosition = Vector2f(x, y);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRotation(float rotation)
{
	m_relRotation = fmodf(rotation, 360.0f);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setSize(const Vector2f& size)
{
	m_pixelSize = size;
	m_useRelSize = Vector2f(false);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setSize(float w, float h)
{
	m_pixelSize = Vector2f(w, h);
	m_useRelSize = Vector2f(false);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelSize(const Vector2f& size)
{
	m_relSize = size;
	m_useRelSize = Vector2f(true);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelSize(float w, float h)
{
	m_relSize = Vector2f(w, h);
	m_useRelSize = Vector2f(true);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setWidth(float w)
{
	m_pixelSize.x = w;
	m_useRelSize.x = false;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setHeight(float h)
{
	m_pixelSize.y = h;
	m_useRelSize.y = false;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelWidth(float w)
{
	m_relSize.x = w;
	m_useRelSize.x = true;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelHeight(float h)
{
	m_relSize.y = h;
	m_useRelSize.y = true;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setOrigin(const Vector2f& origin)
{
	m_origin = origin;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setOrigin(float x, float y)
{
	m_origin = Vector2f(x, y);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setOrigin(UIPosition origin)
{
	m_origin = priv::getPositionFromEnum(origin);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setAnchor(const Vector2f& anchor)
{
	m_anchor = anchor;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setAnchor(float x, float y)
{
	m_anchor = Vector2f(x, y);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setAnchor(UIPosition anchor)
{
	m_anchor = priv::getPositionFromEnum(anchor);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setColor(const Vector4f& color)
{
	m_color = color;
	m_isColorTransparent = m_color.a < 1.0f - FLT_EPSILON;
}


///////////////////////////////////////////////////////////
void UIElement::setColor(float r, float g, float b, float a)
{
	m_color = Vector4f(r, g, b, a);
	m_isColorTransparent = m_color.a < 1.0f - FLT_EPSILON;
}


///////////////////////////////////////////////////////////
void UIElement::setTexture(Texture* texture)
{
	m_texture = texture;
}


///////////////////////////////////////////////////////////
void UIElement::setTextureRect(const Vector4f& rect)
{
	m_textureRect = rect;
}


///////////////////////////////////////////////////////////
void UIElement::setTextureRect(float x, float y, float w, float h)
{
	m_textureRect = Vector4f(x, y, w, h);
}


///////////////////////////////////////////////////////////
void UIElement::setFlippedUv(bool flipped)
{
	m_hasFlippedUv = flipped;
}


///////////////////////////////////////////////////////////
void UIElement::setBlendFactors(BlendFactor src, BlendFactor dst)
{
	m_srcBlend = src;
	m_dstBlend = dst;
}


///////////////////////////////////////////////////////////
void UIElement::setShader(Shader* shader)
{
	m_shader = shader;
}


///////////////////////////////////////////////////////////
void UIElement::setVisible(bool visible, bool recursive)
{
	m_isVisible = visible;

	// Set visibility for all children if recursive
	if (recursive)
	{
		for (Uint32 i = 0; i < m_children.size(); ++i)
			m_children[i]->setVisible(visible, recursive);
	}
}


///////////////////////////////////////////////////////////
void UIElement::setTransparent(bool transparent)
{
	m_isTextureTransparent = transparent;
}


///////////////////////////////////////////////////////////
void UIElement::move(const Vector2f& offset)
{
	m_relPosition += offset;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::move(float x, float y)
{
	m_relPosition += Vector2f(x, y);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::rotate(float angle)
{
	m_relRotation = fmodf(m_relRotation + angle, 360.0f);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::scale(const Vector2f& scale)
{
	m_pixelSize *= scale;
	m_relSize *= scale;
	markTransformDirty();
}


///////////////////////////////////////////////////////////
void UIElement::scale(float w, float h)
{
	m_pixelSize *= Vector2f(w, h);
	m_relSize *= Vector2f(w, h);
	markTransformDirty();
}


///////////////////////////////////////////////////////////
const std::string& UIElement::getId() const
{
	return m_id;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getRelPosition() const
{
	return m_relPosition;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getAbsPosition()
{
	updateTransforms();
	return m_absPosition;
}


///////////////////////////////////////////////////////////
float UIElement::getRelRotation() const
{
	return m_relRotation;
}


///////////////////////////////////////////////////////////
float UIElement::getAbsRotation()
{
	updateTransforms();
	return m_absRotation;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getPixelSize()
{
	updateTransforms();
	return m_pixelSize;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getRelSize()
{
	updateTransforms();
	return m_relSize;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getOrigin() const
{
	return m_origin;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getAnchor() const
{
	return m_anchor;
}


///////////////////////////////////////////////////////////
const Vector4f& UIElement::getColor() const
{
	return m_color;
}


///////////////////////////////////////////////////////////
Texture* UIElement::getTexture() const
{
	return m_texture;
}


///////////////////////////////////////////////////////////
const Vector4f& UIElement::getTextureRect() const
{
	return m_textureRect;
}


///////////////////////////////////////////////////////////
bool UIElement::hasFlippedUv() const
{
	return m_hasFlippedUv;
}


///////////////////////////////////////////////////////////
BlendFactor UIElement::getSrcBlend() const
{
	return m_srcBlend;
}


///////////////////////////////////////////////////////////
BlendFactor UIElement::getDstBlend() const
{
	return m_dstBlend;
}


///////////////////////////////////////////////////////////
Shader* UIElement::getShader() const
{
	return m_shader;
}


///////////////////////////////////////////////////////////
Vector4f UIElement::getClipRect()
{
	return Vector4f(0.0f);
}


///////////////////////////////////////////////////////////
bool UIElement::isVisible() const
{
	return m_isVisible;
}


///////////////////////////////////////////////////////////
bool UIElement::isTransparent() const
{
	return m_isColorTransparent || m_isTextureTransparent;
}


///////////////////////////////////////////////////////////
UIElement* UIElement::getParent() const
{
	return m_parent;
}


///////////////////////////////////////////////////////////
const std::vector<UIElement*>& UIElement::getChildren() const
{
	return m_children;
}


///////////////////////////////////////////////////////////
Uint32 UIElement::getIndex() const
{
	return m_index;
}


///////////////////////////////////////////////////////////
bool UIElement::hasHover() const
{
	return m_hasHover;
}


///////////////////////////////////////////////////////////
bool UIElement::hasFocus() const
{
	return m_hasFocus;
}


///////////////////////////////////////////////////////////
Vector2f UIElement::getLocalCoordinate(const Vector2f& abs)
{
	// Update transforms
	updateTransforms();

	// Adjust for translation
	Vector2f p = abs - m_absPosition;
	// Adjust for rotation
	float angle = rad(m_absRotation);
	float ca = cos(angle);
	float sa = sin(angle);

	// Apply rotation
	p = Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca);

	return p;
}


///////////////////////////////////////////////////////////
void UIElement::getQuads(std::vector<UIQuad>& quads)
{
	// Update transforms
	updateTransforms();

	UIQuad quad;
	quad.m_position = m_absPosition;
	quad.m_rotation = m_absRotation;
	quad.m_size = m_pixelSize;
	quad.m_origin = m_origin;
	quad.m_color = m_color;
	quad.m_textureRect = m_textureRect;

	quads.push_back(quad);
}


///////////////////////////////////////////////////////////
void UIElement::onKeyEvent(const E_KeyEvent& e)
{

}


///////////////////////////////////////////////////////////
void UIElement::onMouseButton(const E_MouseButton& e)
{

}


///////////////////////////////////////////////////////////
void UIElement::onMouseMove(const E_MouseMove& e)
{

}


///////////////////////////////////////////////////////////
void UIElement::onMouseScroll(const E_MouseScroll& e)
{

}


///////////////////////////////////////////////////////////
void UIElement::onTextInput(const E_TextInput& e)
{

}


///////////////////////////////////////////////////////////
void UIElement::onMouseEnter(const E_MouseMove& e)
{

}


///////////////////////////////////////////////////////////
void UIElement::onMouseLeave(const E_MouseMove& e)
{

}


///////////////////////////////////////////////////////////
void UIElement::onGainFocus()
{

}


///////////////////////////////////////////////////////////
void UIElement::onLoseFocus()
{

}


///////////////////////////////////////////////////////////
bool UIElement::handlesMouseEvents() const
{
	return false;
}


///////////////////////////////////////////////////////////
Shader* UIElement::getDefaultShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("shaders/ui.vert", Shader::Vertex);
		s_shader.load("shaders/ui.geom", Shader::Geometry);
		s_shader.load("shaders/ui.frag", Shader::Fragment);
		s_shader.compile();
	}

	return &s_shader;
}


}