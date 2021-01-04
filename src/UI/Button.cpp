#include <poly/Core/ObjectPool.h>

#include <poly/UI/Button.h>
#include <poly/UI/Text.h>

namespace poly
{


///////////////////////////////////////////////////////////
Button::Button() :
	m_text			(Pool<Text>::alloc()),
	m_textAlign		(UIPosition::Center),
	m_isPressed		(false)
{
	// Add child
	addChild(m_text);

	// Set text align
	setTextAlign(m_textAlign);
}


///////////////////////////////////////////////////////////
Button::~Button()
{
	// Free text
	Pool<Text>::free(m_text);
}


///////////////////////////////////////////////////////////
void Button::setString(const std::string& string)
{
	m_text->setString(string);
}


///////////////////////////////////////////////////////////
void Button::setTextAlign(UIPosition align)
{
	m_textAlign = align;

	m_text->setOrigin(align);
	m_text->setAnchor(align);
}


///////////////////////////////////////////////////////////
void Button::setTextOffset(const Vector2f& offset)
{
	m_text->setPosition(offset);
}


///////////////////////////////////////////////////////////
void Button::setTextOffset(float x, float y)
{
	m_text->setPosition(x, y);
}


///////////////////////////////////////////////////////////
const std::string& Button::getString() const
{
	return m_text->getString();
}


///////////////////////////////////////////////////////////
UIPosition Button::getTextAlign() const
{
	return m_textAlign;
}


///////////////////////////////////////////////////////////
const Vector2f& Button::getTextOffset() const
{
	return m_text->getRelPosition();
}


///////////////////////////////////////////////////////////
Text* Button::getText() const
{
	return m_text;
}


///////////////////////////////////////////////////////////
bool Button::isPressed() const
{
	return m_isPressed;
}


///////////////////////////////////////////////////////////
void Button::onMouseEnter(const std::function<void(const E_MouseMove&)>& func)
{
	m_onMouseEnter = func;
}


///////////////////////////////////////////////////////////
void Button::onMouseLeave(const std::function<void(const E_MouseMove&)>& func)
{
	m_onMouseLeave = func;
}


///////////////////////////////////////////////////////////
void Button::onPress(const std::function<void()>& func)
{
	m_onPress = func;
}


///////////////////////////////////////////////////////////
void Button::onRelease(const std::function<void()>& func)
{
	m_onRelease = func;
}


///////////////////////////////////////////////////////////
void Button::onMouseButton(const E_MouseButton& e)
{
	// Only allowed to click with left mouse button
	if (e.m_button != Mouse::Left) return;

	if (e.m_action == InputAction::Press && !m_isPressed)
	{
		m_isPressed = true;

		if (m_onPress)
			m_onPress();
	}
	else if (e.m_action == InputAction::Release && m_isPressed)
	{
		m_isPressed = false;

		if (m_onRelease)
			m_onRelease();
	}
}


///////////////////////////////////////////////////////////
void Button::onMouseEnter(const E_MouseMove& e)
{
	if (m_onMouseEnter)
		m_onMouseEnter(e);
}


///////////////////////////////////////////////////////////
void Button::onMouseLeave(const E_MouseMove& e)
{
	if (m_onMouseLeave)
		m_onMouseLeave(e);
}


///////////////////////////////////////////////////////////
bool Button::handlesMouseEvents() const
{
	return true;
}


}