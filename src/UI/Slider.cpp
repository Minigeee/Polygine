#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Window.h>

#include <poly/UI/Slider.h>
#include <poly/UI/UIParser.h>

namespace poly
{


///////////////////////////////////////////////////////////
SliderButton::SliderButton() :
	m_offset		(0.0f),
	m_isPressed		(false)
{
	setOrigin(UIPosition::Left);
	setAnchor(UIPosition::Left);
}


///////////////////////////////////////////////////////////
void SliderButton::onMouseEnter(const std::function<void(const E_MouseMove&)>& func)
{
	m_onMouseEnter = func;
}


///////////////////////////////////////////////////////////
void SliderButton::onMouseLeave(const std::function<void(const E_MouseMove&)>& func)
{
	m_onMouseLeave = func;
}


///////////////////////////////////////////////////////////
void SliderButton::onMouseButton(const E_MouseButton& e)
{
	// Only handle left clicks
	if (e.m_button != Mouse::Left) return;

	// Only 2 mouse actions
	m_isPressed = e.m_action == InputAction::Press;

	if (m_isPressed)
	{
		// Reset properties
		if (m_relRotation != 0.0f)
			setRotation(0.0f);

		// Calculate actual offset if the slider is pressed
		m_offset = -getLocalCoordinate(Window::getCurrent()->getCursorPos());
	}
}


///////////////////////////////////////////////////////////
void SliderButton::onMouseMove(const E_MouseMove& e)
{
	if (m_isPressed)
	{
		// Convert point to local space, from the slider perspective
		Vector2f p = m_parent->getLocalCoordinate(Vector2f(e.m_x, e.m_y));

		// Update transforms
		updateTransforms();

		// Change the x-position of the slider
		m_relPosition.x = p.x + m_offset.x;

		// Clamp the value
		float max = m_parent->getPixelSize().x - m_pixelSize.x;

		if (m_relPosition.x < 0.0f)
			m_relPosition.x = 0.0f;
		if (m_relPosition.x > max)
			m_relPosition.x = max;

		Slider* slider = (Slider*)m_parent;

		// Check if value changed
		float value = m_relPosition.x / max;
		if (slider->m_value != value)
		{
			// Change value
			slider->m_value = value;
			if (slider->m_onValueChange)
				slider->m_onValueChange(slider->m_value);

			// Mark transform dirty
			markTransformDirty();
		}
	}
}


///////////////////////////////////////////////////////////
void SliderButton::onMouseEnter(const E_MouseMove& e)
{
	if (m_onMouseEnter)
		m_onMouseEnter(e);
}


///////////////////////////////////////////////////////////
void SliderButton::onMouseLeave(const E_MouseMove& e)
{
	if (m_onMouseLeave)
		m_onMouseLeave(e);
}


///////////////////////////////////////////////////////////
bool SliderButton::handlesMouseEvents() const
{
	return true;
}


///////////////////////////////////////////////////////////
Slider::Slider() :
	m_button		(Pool<SliderButton>::alloc()),
	m_value			(0.0f),
	m_isPressed		(false)
{
	// Add as child
	addChild(m_button);

	// By default, the slider button will have the same height as the slider
	m_button->setRelHeight(1.0f);
}


///////////////////////////////////////////////////////////
Slider::~Slider()
{
	Pool<SliderButton>::free(m_button);
	m_button = 0;
}


///////////////////////////////////////////////////////////
void Slider::parse(XmlNode node, const UITemplateMap& templates)
{
	// Default parse
	UIElement::parse(node, templates);

	// Slider value
	XmlAttribute valueAttr = node.getFirstAttribute("value");
	if (valueAttr.exists())
	{
		float value;
		if (UIParser::parse(valueAttr, value))
			setValue(value);
	}

	// On value change
	XmlAttribute onValueChangeAttr = node.getFirstAttribute("on_value_change");
	if (onValueChangeAttr.exists())
	{
		std::function<void(float)> func;
		if (UIParser::parse(onValueChangeAttr, this, func))
			onValueChange(func);
	}

	// Slider button
	XmlNode buttonNode = node.getFirstNode("slider_button");
	if (buttonNode.exists())
		m_button->parse(node, templates);
}


///////////////////////////////////////////////////////////
void Slider::setValue(float value, bool callFunc)
{
	if (value == m_value) return;

	m_value = value;
	if (m_value < 0.0f)
		m_value = 0.0f;
	else if (m_value > 1.0f)
		m_value = 1.0f;

	// Calculate position
	float max = getPixelSize().x - m_button->getPixelSize().x;
	float x = m_value * max;

	// Set button position
	m_button->setPosition(x, m_button->getRelPosition().y);

	// Call function
	if (callFunc && m_onValueChange)
		m_onValueChange(m_value);
}


///////////////////////////////////////////////////////////
float Slider::getValue() const
{
	return m_value;
}


///////////////////////////////////////////////////////////
SliderButton* Slider::getSliderButton()
{
	return m_button;
}


///////////////////////////////////////////////////////////
void Slider::onValueChange(const std::function<void(float)>& func)
{
	m_onValueChange = func;
}


///////////////////////////////////////////////////////////
void Slider::onMouseButton(const E_MouseButton& e)
{
	// Only handle left clicks
	if (e.m_button != Mouse::Left) return;

	// Only 2 mouse actions
	m_isPressed = e.m_action == InputAction::Press;

	if (m_isPressed)
	{
		// Change button position
		Vector2f p = getLocalCoordinate(Window::getCurrent()->getCursorPos());

		// Clamp position
		float min = 0.5f * m_button->getPixelSize().x;
		float max = getPixelSize().x - min;

		if (p.x < min)
			p.x = min;
		if (p.x > max)
			p.x = max;

		m_button->setPosition(p.x - min, m_button->getRelPosition().y);

		// Check if the value changed
		float value = (p.x - min) / (max - min);
		if (m_value != value)
		{
			// Change value
			m_value = value;
			if (m_onValueChange)
				m_onValueChange(m_value);
		}
	}
}


///////////////////////////////////////////////////////////
void Slider::onMouseMove(const E_MouseMove& e)
{
	if (m_isPressed)
	{
		// Change button position
		Vector2f p = getLocalCoordinate(Vector2f(e.m_x, e.m_y));

		// Clamp position
		float min = 0.5f * m_button->getPixelSize().x;
		float max = getPixelSize().x - min;

		if (p.x < min)
			p.x = min;
		if (p.x > max)
			p.x = max;

		m_button->setPosition(p.x - min, m_button->getRelPosition().y);

		// Check if the value changed
		float value = (p.x - min) / (max - min);
		if (m_value != value)
		{
			// Change value
			m_value = value;
			if (m_onValueChange)
				m_onValueChange(m_value);
		}
	}
}


///////////////////////////////////////////////////////////
bool Slider::handlesMouseEvents() const
{
	return true;
}


}