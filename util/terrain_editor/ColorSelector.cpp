#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Window.h>

#include "ColorSelector.h"
#include "UITemplates.h"

#include <iomanip>
#include <string>


///////////////////////////////////////////////////////////
void ColorSelector::onConfirmCancelBtn(Button* btn)
{
	if (btn->hasHover())
	{
		btn->setColor(0.25f, 0.25f, 0.30f, 1.0f);

		setVisible(false);
		if (m_onConfirm && btn == m_confirmBtn)
			m_onConfirm();
	}
	else
		btn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
}


///////////////////////////////////////////////////////////
ColorSelector::ColorSelector() :
	m_panelTitle	(Pool<Text>::alloc()),
	m_rTitle		(Pool<Text>::alloc()),
	m_gTitle		(Pool<Text>::alloc()),
	m_bTitle		(Pool<Text>::alloc()),
	m_hexTitle		(Pool<Text>::alloc()),
	m_decTitle		(Pool<Text>::alloc()),
	m_rSlider		(Pool<Slider>::alloc()),
	m_gSlider		(Pool<Slider>::alloc()),
	m_bSlider		(Pool<Slider>::alloc()),
	m_rInput		(Pool<TextInput>::alloc()),
	m_gInput		(Pool<TextInput>::alloc()),
	m_bInput		(Pool<TextInput>::alloc()),
	m_hexInput		(Pool<TextInput>::alloc()),
	m_decInput		(Pool<TextInput>::alloc()),
	m_sampleBox		(Pool<UIElement>::alloc()),
	m_confirmBtn	(Pool<Button>::alloc()),
	m_cancelBtn		(Pool<Button>::alloc()),
	m_mouseOffset	(0.0f),
	m_isPressed		(false)
{
	setSize(450.0f, 360.0f);
	setColor(0.15f, 0.15f, 0.18f, 1.0f);
	setAnchor(UIPosition::Center);
	setOrigin(UIPosition::Center);

	m_panelTitle->setString("Color Selector");
	m_panelTitle->setOrigin(UIPosition::TopCenter);
	m_panelTitle->setAnchor(UIPosition::TopCenter);
	m_panelTitle->setPosition(0.0f, 10.0f);
	addChild(m_panelTitle);

	m_rTitle->setPosition(8.0f, 35.0f);
	m_rTitle->setString("Red");
	addChild(m_rTitle);

	m_rSlider->setPosition(5.0f, 55.0f);
	m_rSlider->setSize(300.0f, 15.0f);
	m_rSlider->setColor(0.12f, 0.12f, 0.15f, 1.0f);
	m_rSlider->getSliderButton()->setSize(20.0f, 13.0f);
	m_rSlider->getSliderButton()->setColor(0.3f, 0.3f, 0.35f, 1.0f);
	addChild(m_rSlider);

	m_rInput->setAnchor(UIPosition::TopRight);
	m_rInput->setOrigin(UIPosition::TopRight);
	m_rInput->setPosition(-8.0f, 50.0f);
	m_rInput->setSize(75.0f, 25.0f);
	m_rInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	addChild(m_rInput);

	m_gTitle->setPosition(8.0f, 85.0f);
	m_gTitle->setString("Green");
	addChild(m_gTitle);

	m_gSlider->setPosition(5.0f, 105.0f);
	m_gSlider->setSize(300.0f, 15.0f);
	m_gSlider->setColor(0.12f, 0.12f, 0.15f, 1.0f);
	m_gSlider->getSliderButton()->setSize(20.0f, 13.0f);
	m_gSlider->getSliderButton()->setColor(0.3f, 0.3f, 0.35f, 1.0f);
	addChild(m_gSlider);

	m_gInput->setAnchor(UIPosition::TopRight);
	m_gInput->setOrigin(UIPosition::TopRight);
	m_gInput->setPosition(-8.0f, 100.0f);
	m_gInput->setSize(75.0f, 25.0f);
	m_gInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	addChild(m_gInput);

	m_bTitle->setPosition(8.0f, 135.0f);
	m_bTitle->setString("Blue");
	addChild(m_bTitle);

	m_bSlider->setPosition(5.0f, 155.0f);
	m_bSlider->setSize(300.0f, 15.0f);
	m_bSlider->setColor(0.12f, 0.12f, 0.15f, 1.0f);
	m_bSlider->getSliderButton()->setSize(20.0f, 13.0f);
	m_bSlider->getSliderButton()->setColor(0.3f, 0.3f, 0.35f, 1.0f);
	addChild(m_bSlider);

	m_bInput->setAnchor(UIPosition::TopRight);
	m_bInput->setOrigin(UIPosition::TopRight);
	m_bInput->setPosition(-8.0f, 150.0f);
	m_bInput->setSize(75.0f, 25.0f);
	m_bInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	addChild(m_bInput);

	m_hexTitle->setPosition(8.0f, 205.0f);
	m_hexTitle->setString("Hex:");
	addChild(m_hexTitle);

	m_hexInput->setAnchor(UIPosition::TopRight);
	m_hexInput->setOrigin(UIPosition::TopRight);
	m_hexInput->setPosition(-8.0f, 200.0f);
	m_hexInput->setSize(75.0f, 25.0f);
	m_hexInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	addChild(m_hexInput);

	m_decTitle->setPosition(8.0f, 240.0f);
	m_decTitle->setString("Dec:");
	addChild(m_decTitle);

	m_decInput->setAnchor(UIPosition::TopRight);
	m_decInput->setOrigin(UIPosition::TopRight);
	m_decInput->setPosition(-8.0f, 235.0f);
	m_decInput->setSize(75.0f, 25.0f);
	m_decInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	addChild(m_decInput);

	m_sampleBox->setPosition(8.0f, 285.0f);
	m_sampleBox->setSize(434.0f, 25.0f);
	addChild(m_sampleBox);

	m_cancelBtn->setAnchor(UIPosition::BotRight);
	m_cancelBtn->setOrigin(UIPosition::BotRight);
	m_cancelBtn->setPosition(-8.0f, -8.0f);
	m_cancelBtn->setSize(70.0f, 25.0f);
	m_cancelBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_cancelBtn->setString("Cancel");
	m_cancelBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_cancelBtn, std::placeholders::_1));
	m_cancelBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_cancelBtn, std::placeholders::_1));
	m_cancelBtn->onPress(std::bind(onBtnPress, m_cancelBtn));
	m_cancelBtn->onRelease(std::bind(&ColorSelector::onConfirmCancelBtn, this, m_cancelBtn));
	addChild(m_cancelBtn);

	m_confirmBtn->setAnchor(UIPosition::BotRight);
	m_confirmBtn->setOrigin(UIPosition::BotRight);
	m_confirmBtn->setPosition(-84.0f, -8.0f);
	m_confirmBtn->setSize(70.0f, 25.0f);
	m_confirmBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_confirmBtn->setString("OK");
	m_confirmBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_confirmBtn, std::placeholders::_1));
	m_confirmBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_confirmBtn, std::placeholders::_1));
	m_confirmBtn->onPress(std::bind(onBtnPress, m_confirmBtn));
	m_confirmBtn->onRelease(std::bind(&ColorSelector::onConfirmCancelBtn, this, m_confirmBtn));
	addChild(m_confirmBtn);

	// Red slider
	m_rSlider->onValueChange(
		[&](float value)
		{
			setSelectedColor(value, m_selectedColor.g, m_selectedColor.b);
		}
	);

	// Red input
	m_rInput->onValueChange(
		[&](const std::string& value)
		{
			int num = 0;
			try
			{
				num = std::stoi(value);
			}
			catch (const std::exception&) {}

			// Clamp number
			if (num < 0)
				num = 0;
			else if (num > 255)
				num = 255;

			setSelectedColor((float)num / 255.0f, m_selectedColor.g, m_selectedColor.b);
		}
	);

	// Green slider
	m_gSlider->onValueChange(
		[&](float value)
		{
			setSelectedColor(m_selectedColor.r, value, m_selectedColor.b);
		}
	);

	// Green input
	m_gInput->onValueChange(
		[&](const std::string& value)
		{
			int num = 0;
			try
			{
				num = std::stoi(value);
			}
			catch (const std::exception&) {}

			// Clamp number
			if (num < 0)
				num = 0;
			else if (num > 255)
				num = 255;

			setSelectedColor(m_selectedColor.r, (float)num / 255.0f, m_selectedColor.b);
		}
	);

	// Blue slider
	m_bSlider->onValueChange(
		[&](float value)
		{
			setSelectedColor(m_selectedColor.r, m_selectedColor.g, value);
		}
	);

	// Blue input
	m_bInput->onValueChange(
		[&](const std::string& value)
		{
			int num = 0;
			try
			{
				num = std::stoi(value);
			}
			catch (const std::exception&) {}

			// Clamp number
			if (num < 0)
				num = 0;
			else if (num > 255)
				num = 255;

			setSelectedColor(m_selectedColor.r, m_selectedColor.g, (float)num / 255.0f);
		}
	);

	// Hex input
	m_hexInput->onValueChange(
		[&](const std::string& value)
		{
			if (value.size() != 7) return;

			int r = 0, g = 0, b = 0;
			try
			{
				r = std::strtol(value.substr(1, 2).c_str(), NULL, 16);
				g = std::strtol(value.substr(3, 2).c_str(), NULL, 16);
				b = std::strtol(value.substr(5, 2).c_str(), NULL, 16);
			}
			catch (const std::exception&) {}

			// Clamp number
			if (r < 0) r = 0;
			else if (r > 255) r = 255;

			if (g < 0) g = 0;
			else if (g > 255) g = 255;

			if (b < 0) b = 0;
			else if (b > 255) b = 255;

			setSelectedColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
		}
	);

	// Dec input
	m_decInput->onValueChange(
		[&](const std::string& value)
		{
			int r = 0, g = 0, b = 0;
			try
			{
				// Find comma positions
				int comma1 = -1, comma2 = -1;
				for (Uint32 i = 0; i < value.size() && comma2 < 0; ++i)
				{
					if (value[i] == ',')
					{
						if (comma1 >= 0)
							comma2 = i;
						else
							comma1 = i;
					}
				}

				if (comma1 < 0 || comma2 < 0 || comma2 < comma1) return;

				r = std::strtol(value.substr(0, comma1).c_str(), NULL, 10);
				g = std::strtol(value.substr(comma1 + 1, comma2 - comma1 - 1).c_str(), NULL, 10);
				b = std::strtol(value.substr(comma2 + 1).c_str(), NULL, 10);
			}
			catch (const std::exception&) {}

			// Clamp number
			if (r < 0) r = 0;
			else if (r > 255) r = 255;

			if (g < 0) g = 0;
			else if (g > 255) g = 255;

			if (b < 0) b = 0;
			else if (b > 255) b = 255;

			setSelectedColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
		}
	);

	// Set default color
	setSelectedColor(0.0f, 0.0f, 0.0f);

	// Hide by default
	setVisible(false);
}


///////////////////////////////////////////////////////////
ColorSelector::~ColorSelector()
{
	Pool<Text>::free(m_panelTitle);
	Pool<Text>::free(m_rTitle);
	Pool<Text>::free(m_gTitle);
	Pool<Text>::free(m_bTitle);
	Pool<Text>::free(m_hexTitle);
	Pool<Text>::free(m_decTitle);
	Pool<Slider>::free(m_rSlider);
	Pool<Slider>::free(m_gSlider);
	Pool<Slider>::free(m_bSlider);
	Pool<TextInput>::free(m_rInput);
	Pool<TextInput>::free(m_gInput);
	Pool<TextInput>::free(m_bInput);
	Pool<TextInput>::free(m_hexInput);
	Pool<TextInput>::free(m_decInput);
	Pool<UIElement>::free(m_sampleBox);
	Pool<Button>::free(m_confirmBtn);
	Pool<Button>::free(m_cancelBtn);
}


///////////////////////////////////////////////////////////
void ColorSelector::setSelectedColor(float r, float g, float b)
{
	m_selectedColor = Vector3f(r, g, b);

	Uint32 rd = (Uint32)(r * 255.0f);
	Uint32 gd = (Uint32)(g * 255.0f);
	Uint32 bd = (Uint32)(b * 255.0f);

	// Set color component slider values
	m_rSlider->setValue(r);
	m_gSlider->setValue(g);
	m_bSlider->setValue(b);

	// Set color component input strings
	m_rInput->setValue(std::to_string(rd));
	m_gInput->setValue(std::to_string(gd));
	m_bInput->setValue(std::to_string(bd));

	// Set color input values
	char hex[10];
	char dec[20];
	sprintf(hex, "#%02X%02X%02X", rd, gd, bd);
	sprintf(dec, "%d,%d,%d", rd, gd, bd);
	m_hexInput->setValue(hex);
	m_decInput->setValue(dec);

	// Change sample color box
	m_sampleBox->setColor(r, g, b, 1.0f);
}


///////////////////////////////////////////////////////////
const Vector3f& ColorSelector::getSelectedColor() const
{
	return m_selectedColor;
}


///////////////////////////////////////////////////////////
void ColorSelector::onMouseButton(const E_MouseButton& e)
{
	// Left click in top margin
	if (e.m_button != Mouse::Left) return;

	if (e.m_action == InputAction::Press)
	{
		const float margin = 25.0f;
		Vector2f offset = getLocalCoordinate(Window::getCurrent()->getCursorPos());
		float localY = offset.y + m_origin.y * getPixelSize().y;

		if (localY < margin)
		{
			// Keep track of offset
			m_mouseOffset = offset;
			m_isPressed = true;
		}
	}
	else
		m_isPressed = false;
}


///////////////////////////////////////////////////////////
void ColorSelector::onMouseMove(const E_MouseMove& e)
{
	if (m_isPressed)
	{
		// Get position relative to parent
		Vector2f p = m_parent->getLocalCoordinate(Vector2f(e.m_x, e.m_y));

		// Set position to mouse position plus original offset
		setPosition(p - m_mouseOffset - m_anchor * m_parent->getPixelSize());
	}
}


///////////////////////////////////////////////////////////
bool ColorSelector::handlesMouseEvents() const
{
	return true;
}


///////////////////////////////////////////////////////////
void ColorSelector::onConfirm(const std::function<void()>& func)
{
	m_onConfirm = func;
}