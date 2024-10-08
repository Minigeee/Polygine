#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Window.h>

#include <poly/Math/Functions.h>

#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>
#include <poly/UI/UIParser.h>

namespace poly
{


///////////////////////////////////////////////////////////
TextInput::TextInput() :
	m_text				(Pool<Text>::alloc()),
	m_textCursor		(Pool<UIElement>::alloc()),
	m_highlight			(Pool<UIElement>::alloc()),
	m_cursorCycle		(1.2f),
	m_time				(0.0f),
	m_submitKey			(Keyboard::Enter),
	m_cursorCharPos		(0),
	m_textSelection		(0),
	m_textAlign			(UIPosition::Left),
	m_isPressed			(false)
{
	// Setup highlight
	addChild(m_highlight);
	m_highlight->setColor(0.30f, 0.53f, 0.64f, 1.0f);
	m_highlight->setVisible(false, false);

	// Setup text
	addChild(m_text);
	m_text->setPosition(5.0f, 0.0f);
	m_textCursor->setVisible(false, false);

	// Setup text cursor
	addChild(m_textCursor);
	m_textCursor->setPosition(5.0f, 0.0f);
	m_textCursor->setSize(1.0f, 15.0f);

	// Set alignment
	setTextAlign(m_textAlign);
}


///////////////////////////////////////////////////////////
TextInput::~TextInput()
{
	Pool<Text>::free(m_text);
	m_text = 0;
}


///////////////////////////////////////////////////////////
void TextInput::update(float dt)
{
	if (m_hasFocus)
	{
		// Keep track of time
		m_time = fmodf(m_time + dt, m_cursorCycle);

		// Set visibility
		m_textCursor->setVisible(m_time < 0.5f * m_cursorCycle, false);
	}
}


///////////////////////////////////////////////////////////
void TextInput::parse(XmlNode node, const UITemplateMap& templates)
{
	// Default parse
	UIElement::parse(node, templates);

	// Text value
	XmlAttribute valueAttr = node.getFirstAttribute("value");
	if (valueAttr.exists())
		setValue(valueAttr.getValue());

	else
	{
		XmlAttribute textAttr = node.getFirstAttribute("text");
		if (textAttr.exists())
			setValue(textAttr.getValue());
	}

	// Text cursor size
	XmlAttribute cursorSizeAttr = node.getFirstAttribute("cursor_size");
	if (cursorSizeAttr.exists())
	{
		Vector2f size;
		if (UIParser::parse(cursorSizeAttr, size))
			setTextCursorSize(size);
	}

	// Text cursor color
	XmlAttribute cursorColorAttr = node.getFirstAttribute("cursor_color");
	if (cursorColorAttr.exists())
	{
		Vector4f color;
		if (UIParser::parseColor(cursorColorAttr, color))
			setTextCursorColor(color);
	}

	// Text cursor cycle
	XmlAttribute cursorCycleAttr = node.getFirstAttribute("cursor_cycle");
	if (cursorCycleAttr.exists())
	{
		float cycle;
		if (UIParser::parse(cursorCycleAttr, cycle))
			setTextCursorCycle(cycle);
	}

	// Highlight cursor color
	XmlAttribute highlightColorAttr = node.getFirstAttribute("highlight_color");
	if (highlightColorAttr.exists())
	{
		Vector4f color;
		if (UIParser::parseColor(cursorColorAttr, color))
			setHighlightColor(color);
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

	// On value change
	XmlAttribute onValueChangeAttr = node.getFirstAttribute("on_value_change");
	if (onValueChangeAttr.exists())
	{
		std::function<void(const Utf32String&)> func;
		if (UIParser::parse(onValueChangeAttr, this, func))
			onValueChange(func);
	}

	// On gain focus
	XmlAttribute onGainFocusAttr = node.getFirstAttribute("on_gain_focus");
	if (onGainFocusAttr.exists())
	{
		std::function<void()> func;
		if (UIParser::parse(onGainFocusAttr, this, func))
			onGainFocus(func);
	}

	// On lose focus
	XmlAttribute onLoseFocusAttr = node.getFirstAttribute("on_lose_focus");
	if (onLoseFocusAttr.exists())
	{
		std::function<void()> func;
		if (UIParser::parse(onLoseFocusAttr, this, func))
			onLoseFocus(func);
	}

	// On submit
	XmlAttribute onSubmitAttr = node.getFirstAttribute("on_submit");
	if (onSubmitAttr.exists())
	{
		std::function<void(const Utf32String&)> func;
		if (UIParser::parse(onSubmitAttr, this, func))
			onSubmit(func);
	}

	// Parse text options
	XmlNode textNode = node.getFirstNode("input_text");
	if (textNode.exists())
	{
		getText()->parse(textNode, templates);

		// Reset the text id because it shouldn't have an id
		getText()->setId("");
	}
}


///////////////////////////////////////////////////////////
void TextInput::submit()
{
	if (m_onSubmit)
		m_onSubmit(m_text->getString());
}


///////////////////////////////////////////////////////////
void TextInput::setValue(const std::string& value, bool callFunc)
{
	Utf32String utf32 = Utf32::fromUtf8(value);

	if (utf32 != m_text->getString())
	{
		m_text->setString(value);

		if (callFunc && m_onValueChanged)
			m_onValueChanged(utf32);
	}
}


///////////////////////////////////////////////////////////
void TextInput::setSubmitKey(Keyboard key)
{
	m_submitKey = key;
}


///////////////////////////////////////////////////////////
void TextInput::setTextCursorSize(const Vector2f& size)
{
	m_textCursor->setSize(size);
}

///////////////////////////////////////////////////////////
void TextInput::setTextCursorSize(float x, float y)
{
	m_textCursor->setSize(x, y);
}


///////////////////////////////////////////////////////////
void TextInput::setTextCursorColor(const Vector4f& color)
{
	m_textCursor->setColor(color);
}


///////////////////////////////////////////////////////////
void TextInput::setTextCursorColor(float r, float g, float b, float a)
{
	m_textCursor->setColor(r, g, b, a);
}


///////////////////////////////////////////////////////////
void TextInput::setTextCursorPosition(Uint32 pos)
{
	if (m_cursorCharPos != pos)
	{
		m_cursorCharPos = pos;

		// Update cursor
		m_textCursor->setPosition(m_text->getRelPosition() + Vector2f(m_text->getCharacterOffset(m_cursorCharPos).x, 0.0f));
	}

	// Update selection start position
	m_selectStart = pos;

	// Reset time
	m_time = 0.0f;

	// Clear any text selection
	if (m_textSelection.y - m_textSelection.x > 0)
		m_highlight->setWidth(0.0f);
	m_textSelection = Vector2u(m_cursorCharPos);
}


///////////////////////////////////////////////////////////
void TextInput::setTextCursorCycle(float cycle)
{
	m_cursorCycle = cycle;
}


///////////////////////////////////////////////////////////
void TextInput::setHighlightColor(const Vector4f& color)
{
	m_highlight->setColor(color);
}


///////////////////////////////////////////////////////////
void TextInput::setHighlightColor(float r, float g, float b, float a)
{
	m_highlight->setColor(r, g, b, a);
}


///////////////////////////////////////////////////////////
void TextInput::setTextSelection(Uint32 start, Uint32 end)
{
	m_textSelection = Vector2u(start, end);
	m_cursorCharPos = end;
	m_selectStart = start;

	// Update highlight
	float startPos = m_text->getCharacterOffset(m_textSelection.x).x;
	float endPos = m_text->getCharacterOffset(m_textSelection.y).x;
	m_highlight->setPosition(m_text->getRelPosition() + Vector2f(startPos, 0.0f));
	m_highlight->setSize(endPos - startPos, m_textCursor->getPixelSize().y);

	// Update cursor
	m_textCursor->setPosition(m_text->getRelPosition() + Vector2f(endPos, 0.0f));
}


///////////////////////////////////////////////////////////
void TextInput::setTextAlign(UIPosition align)
{
	m_textAlign = align;

	m_text->setAnchor(m_textAlign);
	m_text->setOrigin(m_textAlign);
	m_textCursor->setAnchor(m_textAlign);
	m_textCursor->setOrigin(m_textAlign);
	m_highlight->setAnchor(m_textAlign);
	m_highlight->setOrigin(m_textAlign);
}


///////////////////////////////////////////////////////////
void TextInput::setTextOffset(const Vector2f& offset)
{
	m_text->setPosition(offset);
}


///////////////////////////////////////////////////////////
void TextInput::setTextOffset(float x, float y)
{
	m_text->setPosition(x, y);
}


///////////////////////////////////////////////////////////
void TextInput::setVisible(bool visible, bool recursive)
{
	bool origVal = m_textCursor->isVisible();

	// Default function
	UIElement::setVisible(visible, recursive);

	// Set text cursor to its original state
	m_textCursor->setVisible(origVal, false);
}


///////////////////////////////////////////////////////////
const Utf32String& TextInput::getValue() const
{
	return m_text->getString();
}


///////////////////////////////////////////////////////////
Keyboard TextInput::getSubmitKey() const
{
	return m_submitKey;
}


///////////////////////////////////////////////////////////
const Vector2f& TextInput::getTextCursorSize() const
{
	return m_textCursor->getPixelSize();
}


///////////////////////////////////////////////////////////
const Vector4f& TextInput::getTextCursorColor() const
{
	return m_textCursor->getColor();
}


///////////////////////////////////////////////////////////
Uint32 TextInput::getTextCursorPosition() const
{
	return m_cursorCharPos;
}


///////////////////////////////////////////////////////////
float TextInput::getTextCursorCycle() const
{
	return m_cursorCycle;
}


///////////////////////////////////////////////////////////
const Vector4f& TextInput::getHighlightColor() const
{
	return m_highlight->getColor();
}


///////////////////////////////////////////////////////////
const Vector2u& TextInput::getTextSelection() const
{
	return m_textSelection;
}


///////////////////////////////////////////////////////////
UIPosition TextInput::getTextAlign() const
{
	return m_textAlign;
}


///////////////////////////////////////////////////////////
const Vector2f& TextInput::getTextOffset() const
{
	return m_text->getRelPosition();
}


///////////////////////////////////////////////////////////
Text* TextInput::getText()
{
	return m_text;
}


///////////////////////////////////////////////////////////
void TextInput::onValueChange(const std::function<void(const Utf32String&)>& func)
{
	m_onValueChanged = func;
}


///////////////////////////////////////////////////////////
void TextInput::onGainFocus(const std::function<void()>& func)
{
	m_onGainFocus = func;
}


///////////////////////////////////////////////////////////
void TextInput::onLoseFocus(const std::function<void()>& func)
{
	m_onLoseFocus = func;
}


///////////////////////////////////////////////////////////
void TextInput::onSubmit(const std::function<void(const Utf32String&)>& func)
{
	m_onSubmit = func;
}


///////////////////////////////////////////////////////////
void TextInput::onMouseButton(const E_MouseButton& e)
{
	// Only handle left-click events
	if (e.m_button != Mouse::Left) return;

	m_isPressed = e.m_action == InputAction::Press;
	if (m_isPressed)
	{
		Window* window = Window::getCurrent();

		// Change cursor position
		Vector2f p = m_text->getLocalCoordinate(window->getCursorPos());

		// Find correct index in string
		const Utf32String& value = m_text->getString();
		Uint32 i = 0;
		for (; i < value.size() && p.x > m_text->getCharacterOffset(i).x; ++i);

		// Decide which character position to round to
		if (i != 0 && i != value.size() - 1)
		{
			if (p.x < 0.5f * (m_text->getCharacterOffset(i - 1).x + m_text->getCharacterOffset(i).x))
				--i;
		}

		// If shift is held, create a text selection
		if (window->isKeyPressed(Keyboard::LeftShift) || window->isKeyPressed(Keyboard::RightShift))
		{
			m_cursorCharPos = i;

			// Update range
			m_textSelection = Vector2u(i < m_selectStart ? i : m_selectStart, i > m_selectStart ? i : m_selectStart);

			// Update highlight rectangle
			float start = m_text->getCharacterOffset(m_textSelection.x).x;
			float end = m_text->getCharacterOffset(m_textSelection.y).x;
			float cursorPos = m_textSelection.x == m_cursorCharPos ? start : end;
			m_highlight->setPosition(m_text->getRelPosition() + Vector2f(start, 0.0f));
			m_highlight->setSize(end - start, m_textCursor->getPixelSize().y);

			// Update cursor
			m_textCursor->setPosition(m_text->getRelPosition() + Vector2f(cursorPos, 0.0f));

			// Reset blink timer
			m_time = 0.0f;
		}
		else
		{
			// Set cursor position
			setTextCursorPosition(i);
		}
	}
}


///////////////////////////////////////////////////////////
void TextInput::onMouseMove(const E_MouseMove& e)
{
	if (m_isPressed)
	{
		// Change cursor position
		Vector2f p = m_text->getLocalCoordinate(Vector2f(e.m_x, e.m_y));

		// Find correct index in string
		const Utf32String& value = m_text->getString();
		Uint32 i = 0;
		for (; i < value.size() && p.x > m_text->getCharacterOffset(i).x; ++i);

		// Decide which character position to round to
		if (i != 0 && i != value.size() - 1)
		{
			if (p.x < 0.5f * (m_text->getCharacterOffset(i - 1).x + m_text->getCharacterOffset(i).x))
				--i;
		}

		// Check if text cursor position changed
		if (m_cursorCharPos != i)
		{
			m_cursorCharPos = i;

			// Update range
			m_textSelection = Vector2u(i < m_selectStart ? i : m_selectStart, i > m_selectStart ? i : m_selectStart);

			// Update highlight rectangle
			float start = m_text->getCharacterOffset(m_textSelection.x).x;
			float end = m_text->getCharacterOffset(m_textSelection.y).x;
			float cursorPos = m_textSelection.x == m_cursorCharPos ? start : end;
			m_highlight->setPosition(m_text->getRelPosition() + Vector2f(start, 0.0f));
			m_highlight->setSize(end - start, m_textCursor->getPixelSize().y);

			// Update cursor
			m_textCursor->setPosition(m_text->getRelPosition() + Vector2f(cursorPos, 0.0f));

			// Reset blink timer
			m_time = 0.0f;
		}
	}
}


///////////////////////////////////////////////////////////
void TextInput::onMouseEnter(const E_MouseMove& e)
{
	Window::getCurrent()->setCursor(Cursor::IBeam);
}


///////////////////////////////////////////////////////////
void TextInput::onMouseLeave(const E_MouseMove& e)
{
	Window::getCurrent()->setCursor(Cursor::Arrow);
}


///////////////////////////////////////////////////////////
void TextInput::onKeyEvent(const E_KeyEvent& e)
{
	// Ignore all release events
	if (e.m_action == InputAction::Release) return;

	// Check if the submit key was pressed
	if (e.m_action == InputAction::Press && e.m_key == m_submitKey)
	{
		if (m_onSubmit)
			m_onSubmit(m_text->getString());
	}

	// Get window
	Window* window = Window::getCurrent();

	if (window->isKeyPressed(Keyboard::LeftShift) || window->isKeyPressed(Keyboard::RightShift))
	{
		if (e.m_key == Keyboard::Left || e.m_key == Keyboard::Right)
		{
			if (e.m_key == Keyboard::Left && m_cursorCharPos > 0)
				// Update cursor position
				--m_cursorCharPos;

			else if (e.m_key == Keyboard::Right && m_cursorCharPos < m_text->getString().size())
				// Update cursor position
				++m_cursorCharPos;

			// Update range
			m_textSelection = Vector2u(
				m_cursorCharPos < m_selectStart ? m_cursorCharPos : m_selectStart,
				m_cursorCharPos > m_selectStart ? m_cursorCharPos : m_selectStart
			);

			// Update highlight rectangle
			float start = m_text->getCharacterOffset(m_textSelection.x).x;
			float end = m_text->getCharacterOffset(m_textSelection.y).x;
			float cursorPos = m_textSelection.x == m_cursorCharPos ? start : end;
			m_highlight->setPosition(m_text->getRelPosition() + Vector2f(start, 0.0f));
			m_highlight->setSize(end - start, m_textCursor->getPixelSize().y);

			// Update cursor
			m_textCursor->setPosition(m_text->getRelPosition() + Vector2f(cursorPos, 0.0f));
		}
	}

	else if (window->isKeyPressed(Keyboard::LeftControl) || window->isKeyPressed(Keyboard::RightControl))
	{
		if (e.m_action != InputAction::Press) return;

		// Handle copy
		if (e.m_key == Keyboard::C)
		{
			if (m_textSelection.y - m_textSelection.x > 0)
				// Copy to clipboard
				window->setClipboard(Utf8::fromUtf32(m_text->getString().substr(m_textSelection.x, m_textSelection.y - m_textSelection.x)));
		}

		// Handle paste
		else if (e.m_key == Keyboard::V)
		{
			// Update string
			const Utf32String& oldValue = m_text->getString();
			std::string clipboard = window->getClipboard();
			Utf32String newValue = oldValue.substr(0, m_textSelection.x) + Utf32::fromUtf8(clipboard) + oldValue.substr(m_textSelection.y);

			// Set new string
			m_text->setString(newValue);

			// Update text cursor location
			setTextCursorPosition(m_textSelection.x + clipboard.size());

			if (m_onValueChanged)
				m_onValueChanged(newValue);
		}

		// Handle cut
		else if (e.m_key == Keyboard::X)
		{
			if (m_textSelection.y - m_textSelection.x > 0)
			{
				// Copy to clipboard
				window->setClipboard(Utf8::fromUtf32(m_text->getString().substr(m_textSelection.x, m_textSelection.y - m_textSelection.x)));

				// Update string
				const Utf32String& oldValue = m_text->getString();
				Utf32String newValue = oldValue.substr(0, m_textSelection.x) + oldValue.substr(m_textSelection.y);

				// Set new string
				m_text->setString(newValue);

				// Update text cursor location
				setTextCursorPosition(m_textSelection.x);

				if (m_onValueChanged)
					m_onValueChanged(newValue);
			}
		}
	}

	else
	{
		if (e.m_key == Keyboard::Left)
		{
			// Update cursor position
			if (m_textSelection.y - m_textSelection.x > 0)
				setTextCursorPosition(m_textSelection.x);
			else if (m_cursorCharPos > 0)
				setTextCursorPosition(m_cursorCharPos - 1);
		}

		else if (e.m_key == Keyboard::Right)
		{
			// Update cursor position
			if (m_textSelection.y - m_textSelection.x > 0)
				setTextCursorPosition(m_textSelection.y);
			else if (m_cursorCharPos < m_text->getString().size())
				setTextCursorPosition(m_cursorCharPos + 1);
		}

		else if (e.m_key == Keyboard::Backspace)
		{
			const Utf32String& oldValue = m_text->getString();
			Utf32String newValue;

			// Handle backspace
			if (m_cursorCharPos > 0 && m_textSelection.y - m_textSelection.x == 0)
			{
				// Remove the previous character
				newValue = oldValue.substr(0, m_cursorCharPos - 1) + oldValue.substr(m_cursorCharPos);
				m_text->setString(newValue);

				// Set cursor position
				setTextCursorPosition(m_cursorCharPos - 1);

				if (m_onValueChanged)
					m_onValueChanged(newValue);
			}
			else if (m_textSelection.y - m_textSelection.x > 0)
			{
				// Remove the selection
				newValue = oldValue.substr(0, m_textSelection.x) + oldValue.substr(m_textSelection.y);
				m_text->setString(newValue);

				// Set cursor position
				setTextCursorPosition(m_textSelection.x);

				if (m_onValueChanged)
					m_onValueChanged(newValue);
			}
		}

		else if (e.m_key == Keyboard::Delete)
		{
			const Utf32String& oldValue = m_text->getString();
			Utf32String newValue;

			// Handle delete
			if (m_cursorCharPos < oldValue.size() && m_textSelection.y - m_textSelection.x == 0)
			{
				// Remove the previous character
				newValue = oldValue.substr(0, m_cursorCharPos) + oldValue.substr(m_cursorCharPos + 1);
				m_text->setString(newValue);

				if (m_onValueChanged)
					m_onValueChanged(newValue);
			}
			else if (m_textSelection.y - m_textSelection.x > 0)
			{
				// Remove the selection
				newValue = oldValue.substr(0, m_textSelection.x) + oldValue.substr(m_textSelection.y);
				m_text->setString(newValue);

				// Set cursor position
				setTextCursorPosition(m_textSelection.x);

				if (m_onValueChanged)
					m_onValueChanged(newValue);
			}
		}
	}
}


///////////////////////////////////////////////////////////
void TextInput::onTextInput(const E_TextInput& e)
{
	// Update string
	const Utf32String& oldValue = m_text->getString();
	Utf32String newValue = oldValue.substr(0, m_textSelection.x) + e.m_char + oldValue.substr(m_textSelection.y);

	// Set new string
	m_text->setString(newValue);

	// Update text cursor location
	setTextCursorPosition(m_textSelection.x + 1);

	if (m_onValueChanged)
		m_onValueChanged(newValue);
}


///////////////////////////////////////////////////////////
void TextInput::onGainFocus()
{
	// Show input elements
	m_textCursor->setVisible(true, false);
	m_highlight->setVisible(true, false);

	if (m_onGainFocus)
		m_onGainFocus();
}


///////////////////////////////////////////////////////////
void TextInput::onLoseFocus()
{
	// Hide input elements
	m_textCursor->setVisible(false, false);
	m_highlight->setVisible(false, false);

	if (m_onLoseFocus)
		m_onLoseFocus();
}


///////////////////////////////////////////////////////////
bool TextInput::handlesMouseEvents() const
{
	return true;
}


}