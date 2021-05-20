#include <poly/Core/ObjectPool.h>

#include <poly/UI/Button.h>
#include <poly/UI/Text.h>

namespace poly
{


///////////////////////////////////////////////////////////
Button::Button() :
	m_text			(0),
	m_textAlign		(UIPosition::Center),
	m_isPressed		(false)
{

}


///////////////////////////////////////////////////////////
Button::~Button()
{
	// Free text
	Pool<Text>::free(m_text);
}


///////////////////////////////////////////////////////////
void Button::createText()
{
	m_text = Pool<Text>::alloc();
	setTextAlign(m_textAlign);
	addChild(m_text);
}


///////////////////////////////////////////////////////////
void Button::setString(const std::string& string)
{
	if (!m_text) createText();
	m_text->setString(string);
}


///////////////////////////////////////////////////////////
void Button::setTextAlign(UIPosition align)
{
	if (!m_text)
	{
		m_text = Pool<Text>::alloc();
		addChild(m_text);
	}

	m_textAlign = align;

	m_text->setOrigin(align);
	m_text->setAnchor(align);
}


///////////////////////////////////////////////////////////
void Button::setTextOffset(const Vector2f& offset)
{
	if (!m_text) createText();
	m_text->setPosition(offset);
}


///////////////////////////////////////////////////////////
void Button::setTextOffset(float x, float y)
{
	if (!m_text) createText();
	m_text->setPosition(x, y);
}


///////////////////////////////////////////////////////////
const std::string& Button::getString()
{
	if (!m_text) createText();
	return m_text->getString();
}


///////////////////////////////////////////////////////////
UIPosition Button::getTextAlign() const
{
	return m_textAlign;
}


///////////////////////////////////////////////////////////
const Vector2f& Button::getTextOffset()
{
	if (!m_text) createText();
	return m_text->getRelPosition();
}


///////////////////////////////////////////////////////////
Text* Button::getText()
{
	if (!m_text) createText();
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
void Button::onCancel(const std::function<void()>& func)
{
	m_onCancel = func;
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

		if (m_hasHover)
		{
			if (m_onRelease)
				m_onRelease();
		}
		else
		{
			if (m_onCancel)
				m_onCancel();
		}
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