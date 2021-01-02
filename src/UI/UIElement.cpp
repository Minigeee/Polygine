#include <poly/UI/UIElement.h>
#include <iostream>

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
UIElement::UIElement() :
	m_parent				(0),
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
	m_blendFactor			(BlendFactor::SrcAlpha),
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
void UIElement::moveToFront()
{
	// Move to back of list
	setIndex(m_children.size() - 1);
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
		for (Uint32 i = m_index; i < index - 1; ++i)
		{
			m_parent->m_children[i] = m_parent->m_children[i + 1];
			--m_parent->m_children[i]->m_index;
		}
	}
	else
	{
		for (Uint32 i = m_index; i > index; --i)
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
void UIElement::markDirty()
{
	m_transformChanged = true;

	for (Uint32 i = 0; i < m_children.size(); ++i)
		m_children[i]->markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::updateProperties()
{
	if (m_transformChanged)
	{
		m_absPosition = m_relPosition;
		m_absRotation = m_relRotation;

		if (m_parent)
		{
			// Update parent transforms
			m_parent->updateProperties();

			Vector2f parentOrigin = m_parent->m_origin * m_parent->m_pixelSize;
			Vector2f anchor = m_anchor * m_parent->m_pixelSize;

			m_absPosition += m_parent->m_absPosition - parentOrigin + anchor;
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
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setPosition(float x, float y)
{
	m_relPosition = Vector2f(x, y);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRotation(float rotation)
{
	m_relRotation = fmodf(rotation, 360.0f);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setSize(const Vector2f& size)
{
	m_pixelSize = size;
	m_useRelSize = Vector2f(false);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setSize(float w, float h)
{
	m_pixelSize = Vector2f(w, h);
	m_useRelSize = Vector2f(false);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelSize(const Vector2f& size)
{
	m_relSize = size;
	m_useRelSize = Vector2f(true);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelSize(float w, float h)
{
	m_relSize = Vector2f(w, h);
	m_useRelSize = Vector2f(true);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setWidth(float w)
{
	m_pixelSize.x = w;
	m_useRelSize.x = false;
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setHeight(float h)
{
	m_pixelSize.y = h;
	m_useRelSize.y = false;
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelWidth(float w)
{
	m_relSize.x = w;
	m_useRelSize.x = true;
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setRelHeight(float h)
{
	m_relSize.y = h;
	m_useRelSize.y = true;
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setOrigin(const Vector2f& origin)
{
	m_origin = origin;
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setOrigin(float x, float y)
{
	m_origin = Vector2f(x, y);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setOrigin(UIPosition origin)
{
	m_origin = priv::getPositionFromEnum(origin);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setAnchor(const Vector2f& anchor)
{
	m_anchor = anchor;
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setAnchor(float x, float y)
{
	m_anchor = Vector2f(x, y);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::setAnchor(UIPosition anchor)
{
	m_anchor = priv::getPositionFromEnum(anchor);
	markDirty();
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
void UIElement::setBlendFactor(BlendFactor factor)
{
	m_blendFactor = factor;
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
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::move(float x, float y)
{
	m_relPosition += Vector2f(x, y);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::rotate(float angle)
{
	m_relRotation = fmodf(m_relRotation + angle, 360.0f);
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::scale(const Vector2f& scale)
{
	m_pixelSize *= scale;
	m_relSize *= scale;
	markDirty();
}


///////////////////////////////////////////////////////////
void UIElement::scale(float w, float h)
{
	m_pixelSize *= Vector2f(w, h);
	m_relSize *= Vector2f(w, h);
	markDirty();
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getRelPosition() const
{
	return m_relPosition;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getAbsPosition()
{
	updateProperties();
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
	updateProperties();
	return m_absRotation;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getPixelSize()
{
	updateProperties();
	return m_pixelSize;
}


///////////////////////////////////////////////////////////
const Vector2f& UIElement::getRelSize()
{
	updateProperties();
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
BlendFactor UIElement::getBlendFactor() const
{
	return m_blendFactor;
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
void UIElement::getQuads(std::vector<UIQuad>& quads)
{
	// Update transforms
	updateProperties();

	UIQuad quad;
	quad.m_position = m_absPosition;
	quad.m_rotation = m_absRotation;
	quad.m_size = m_pixelSize;
	quad.m_origin = m_origin;
	quad.m_color = m_color;
	quad.m_texture = m_texture;
	quad.m_textureRect = m_textureRect;
	quad.m_blendFactor = m_blendFactor;
	quad.m_transparent = isTransparent();

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


}