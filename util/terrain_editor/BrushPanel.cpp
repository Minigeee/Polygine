#include <poly/Core/ObjectPool.h>

#include "BrushPanel.h"

#include <string>


///////////////////////////////////////////////////////////
void brushDropdownHover(Button* button, const E_MouseMove& e)
{
	button->setColor(0.12f, 0.12f, 0.15f, 1.0f);
}


///////////////////////////////////////////////////////////
void brushDropdownUnhover(Button* button, const E_MouseMove& e)
{
	button->setColor(0.1f, 0.1f, 0.12f, 1.0f);
}


///////////////////////////////////////////////////////////
void brushDropdownItemHover(Button* button, const E_MouseMove& e)
{
	button->setColor(0.2f, 0.2f, 0.25f, 1.0f);
}


///////////////////////////////////////////////////////////
void brushDropdownItemUnhover(Button* button, const E_MouseMove& e)
{
	button->setColor(0.18f, 0.18f, 0.2f, 1.0f);
}


///////////////////////////////////////////////////////////
BrushPanel::BrushPanel() :
	m_panelTitle		(Pool<Text>::alloc()),
	m_modeTitle			(Pool<Text>::alloc()),
	m_functionTitle		(Pool<Text>::alloc()),
	m_radiusTitle		(Pool<Text>::alloc()),
	m_strengthTitle		(Pool<Text>::alloc()),
	m_gradientTitle		(Pool<Text>::alloc()),
	
	m_modeMenu			(Pool<Dropdown>::alloc()),
	m_functionMenu		(Pool<Dropdown>::alloc()),
	m_radiusSlider		(Pool<Slider>::alloc()),
	m_strengthSlider	(Pool<Slider>::alloc()),
	m_gradientSlider	(Pool<Slider>::alloc()),
	m_radiusInput		(Pool<TextInput>::alloc()),
	m_strengthInput		(Pool<TextInput>::alloc()),
	m_gradientInput		(Pool<TextInput>::alloc()),
	m_radiusRow			(Pool<HListView>::alloc()),
	m_strengthRow		(Pool<HListView>::alloc()),
	m_gradientRow		(Pool<HListView>::alloc()),

	m_radius			{ 10.0f, 10.0f, 10.0f },
	m_strength			{ 10.0f, 1.0f, 10.0f },
	m_gradient			{ 2.0f, 2.0f, 2.0f }
{
	float width = 200.0f;
	float offsetX = 8.0f;

	m_panelTitle->setOrigin(UIPosition::TopCenter);
	m_panelTitle->setPosition(100.0f, 0.0f);
	m_panelTitle->setString("Brush Settings");
	addChild(m_panelTitle, Vector2f(10.0f, 0.0f));

	m_modeTitle->setPosition(offsetX, 0.0f);
	m_modeTitle->setString("Mode");
	addChild(m_modeTitle, Vector2f(10.0f, 0.0f));

	m_modeMenu->setPosition(offsetX - 5.0f, 0.0f);
	m_modeMenu->setSize(width - 2.0f * offsetX + 6.0f, 25.0f);
	m_modeMenu->setColor(0.1f, 0.1f, 0.12f, 1.0f);
	m_modeMenu->setItemHeight(25.0f);
	m_modeMenu->setItemColor(0.18f, 0.18f, 0.2f, 1.0f);
	m_modeMenu->onMouseEnter(std::bind(brushDropdownHover, m_modeMenu, std::placeholders::_1));
	m_modeMenu->onMouseLeave(std::bind(brushDropdownUnhover, m_modeMenu, std::placeholders::_1));
	m_modeMenu->onMouseEnterItem(brushDropdownItemHover);
	m_modeMenu->onMouseLeaveItem(brushDropdownItemUnhover);
	m_modeMenu->addItem("Height");
	m_modeMenu->addItem("Color");
	m_modeMenu->addItem("Detail");
	addChild(m_modeMenu, Vector2f(3.0f, 0.0f));

	m_functionTitle->setPosition(offsetX, 0.0f);
	m_functionTitle->setString("Function");
	addChild(m_functionTitle, Vector2f(8.0f, 0.0f));

	m_functionMenu->setPosition(offsetX - 5.0f, 0.0f);
	m_functionMenu->setSize(width - 2.0f * offsetX + 6.0f, 25.0f);
	m_functionMenu->setColor(0.1f, 0.1f, 0.12f, 1.0f);
	m_functionMenu->setItemHeight(25.0f);
	m_functionMenu->setItemColor(0.18f, 0.18f, 0.2f, 1.0f);
	m_functionMenu->onMouseEnter(std::bind(brushDropdownHover, m_functionMenu, std::placeholders::_1));
	m_functionMenu->onMouseLeave(std::bind(brushDropdownUnhover, m_functionMenu, std::placeholders::_1));
	m_functionMenu->onMouseEnterItem(brushDropdownItemHover);
	m_functionMenu->onMouseLeaveItem(brushDropdownItemUnhover);
	m_functionMenu->addItem("Add");
	m_functionMenu->addItem("Subtract");
	m_functionMenu->addItem("Smooth");
	addChild(m_functionMenu, Vector2f(3.0f, 0.0f));

	m_radiusTitle->setPosition(offsetX, 0.0f);
	m_radiusTitle->setString("Radius");
	addChild(m_radiusTitle, Vector2f(8.0f, 0.0f));

	m_radiusSlider->setPosition(offsetX - 5.0f, 5.0f);
	m_radiusSlider->setSize(120.0f, 15.0f);
	m_radiusSlider->setColor(0.12f, 0.12f, 0.15f, 1.0f);
	m_radiusSlider->getSliderButton()->setSize(18.0f, 13.0f);
	m_radiusSlider->getSliderButton()->setColor(0.3f, 0.3f, 0.35f, 1.0f);
	m_radiusRow->addChild(m_radiusSlider, Vector2f(3.0f, 5.0f));

	m_radiusInput->setPosition(offsetX + 120.0f, 0.0f);
	m_radiusInput->setSize(65.0f, 25.0f);
	m_radiusInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	m_radiusRow->addChild(m_radiusInput);
	addChild(m_radiusRow, Vector2f(3.0f, 0.0f));

	m_strengthTitle->setPosition(offsetX, 0.0f);
	m_strengthTitle->setString("Strength");
	addChild(m_strengthTitle, Vector2f(8.0f, 0.0f));

	m_strengthSlider->setPosition(offsetX - 5.0f, 5.0f);
	m_strengthSlider->setSize(120.0f, 15.0f);
	m_strengthSlider->setColor(0.12f, 0.12f, 0.15f, 1.0f);
	m_strengthSlider->getSliderButton()->setSize(18.0f, 13.0f);
	m_strengthSlider->getSliderButton()->setColor(0.3f, 0.3f, 0.35f, 1.0f);
	m_strengthRow->addChild(m_strengthSlider, Vector2f(3.0f, 5.0f));

	m_strengthInput->setPosition(offsetX + 120.0f, 0.0f);
	m_strengthInput->setSize(65.0f, 25.0f);
	m_strengthInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	m_strengthRow->addChild(m_strengthInput);
	addChild(m_strengthRow, Vector2f(3.0f, 0.0f));

	m_gradientTitle->setPosition(offsetX, 0.0f);
	m_gradientTitle->setString("Gradient Factor");
	addChild(m_gradientTitle, Vector2f(8.0f, 0.0f));

	m_gradientSlider->setPosition(offsetX - 5.0f, 5.0f);
	m_gradientSlider->setSize(120.0f, 15.0f);
	m_gradientSlider->setColor(0.12f, 0.12f, 0.15f, 1.0f);
	m_gradientSlider->getSliderButton()->setSize(18.0f, 13.0f);
	m_gradientSlider->getSliderButton()->setColor(0.3f, 0.3f, 0.35f, 1.0f);
	m_gradientRow->addChild(m_gradientSlider, Vector2f(3.0f, 5.0f));

	m_gradientInput->setPosition(offsetX + 120.0f, 0.0f);
	m_gradientInput->setSize(65.0f, 25.0f);
	m_gradientInput->setColor(0.25f, 0.25f, 0.3f, 1.0f);
	m_gradientRow->addChild(m_gradientInput);
	addChild(m_gradientRow, Vector2f(3.0f, 0.0f));

	// Radius slider
	m_radiusSlider->onValueChange(
		[&](float value)
		{
			const float min = 1.0f;
			const float max = 100.0f;

			float scaled = value * (max - min) + min;
			m_radius[m_modeMenu->getSelectedItem()] = scaled;

			char buffer[10];
			sprintf(buffer, "%.1f", scaled);

			// Set text input
			m_radiusInput->setValue(std::string(buffer));
		}
	);

	// Radius input
	m_radiusInput->onValueChange(
		[&](const std::string& value)
		{
			float num = 1.0f;
			try
			{
				num = std::stof(value);
			}
			catch (const std::exception&) { }

			const float min = 1.0f;
			const float max = 100.0f;

			float scaled = (num - min) / (max - min);
			m_radius[m_modeMenu->getSelectedItem()] = num;

			// Set slider
			m_radiusSlider->setValue(scaled);
		}
	);

	// Strength slider
	m_strengthSlider->onValueChange(
		[&](float value)
		{
			m_strength[m_modeMenu->getSelectedItem()] = value;

			char buffer[10];
			sprintf(buffer, "%.3f", value);

			// Set text input
			m_strengthInput->setValue(std::string(buffer));
		}
	);

	// Strength input
	m_strengthInput->onValueChange(
		[&](const std::string& value)
		{
			float num = 0.0f;
			try
			{
				num = std::stof(value);
			}
			catch (const std::exception&) {}

			m_strength[m_modeMenu->getSelectedItem()] = num;

			// Set slider
			m_strengthSlider->setValue(num);
		}
	);

	// Gradient slider
	m_gradientSlider->onValueChange(
		[&](float value)
		{
			float scaled = 20.0f * value;
			m_gradient[m_modeMenu->getSelectedItem()] = scaled;

			char buffer[10];
			sprintf(buffer, "%.2f", scaled);

			// Set text input
			m_gradientInput->setValue(std::string(buffer));
		}
	);

	// Strength input
	m_gradientInput->onValueChange(
		[&](const std::string& value)
		{
			float num = 0.0f;
			try
			{
				num = std::stof(value);
			}
			catch (const std::exception&) {}

			float scaled = num * 0.05f;
			m_gradient[m_modeMenu->getSelectedItem()] = num;

			// Set slider
			m_gradientSlider->setValue(scaled);
		}
	);

	// Change mode
	m_modeMenu->onItemChange(
		[&](Uint32 index)
		{
			setMode(index);
		}
	);
}


///////////////////////////////////////////////////////////
BrushPanel::~BrushPanel()
{
	Pool<Text>::free(m_panelTitle);
	Pool<Text>::free(m_modeTitle);
	Pool<Text>::free(m_radiusTitle);
	Pool<Text>::free(m_strengthTitle);
	Pool<Text>::free(m_gradientTitle);

	Pool<Dropdown>::free(m_modeMenu);
	Pool<Dropdown>::free(m_functionMenu);
	Pool<Slider>::free(m_radiusSlider);
	Pool<Slider>::free(m_strengthSlider);
	Pool<Slider>::free(m_gradientSlider);
	Pool<TextInput>::free(m_radiusInput);
	Pool<TextInput>::free(m_strengthInput);
	Pool<TextInput>::free(m_gradientInput);
}


///////////////////////////////////////////////////////////
void BrushPanel::setMode(Uint32 mode)
{
	setRadius(m_radius[mode]);
	setStrength(m_strength[mode]);
	setGradient(m_gradient[mode]);
}


///////////////////////////////////////////////////////////
void BrushPanel::setRadius(float radius)
{
	Uint32 mode = m_modeMenu->getSelectedItem();
	const float min = 1.0f;
	const float max = 100.0f;

	float scaled = (radius - min) / (max - min);
	m_radius[mode] = radius;

	// String number
	char buffer[10];
	sprintf(buffer, "%.1f", radius);

	// Update UI elements
	m_radiusSlider->setValue(scaled);
	m_radiusInput->setValue(std::string(buffer));
}


///////////////////////////////////////////////////////////
void BrushPanel::setStrength(float strength)
{
	Uint32 mode = m_modeMenu->getSelectedItem();
	m_strength[mode] = strength;

	// String number
	char buffer[10];
	sprintf(buffer, "%.3f", strength);

	// Update UI elements
	m_strengthSlider->setValue(strength);
	m_strengthInput->setValue(std::string(buffer));
}


///////////////////////////////////////////////////////////
void BrushPanel::setGradient(float gradient)
{
	Uint32 mode = m_modeMenu->getSelectedItem();

	float scaled = gradient * 0.05f;
	m_gradient[mode] = gradient;

	// String number
	char buffer[10];
	sprintf(buffer, "%.2f", gradient);

	// Update UI elements
	m_gradientSlider->setValue(scaled);
	m_gradientInput->setValue(std::string(buffer));
}


///////////////////////////////////////////////////////////
Uint32 BrushPanel::getMode() const
{
	return m_modeMenu->getSelectedItem();
}


///////////////////////////////////////////////////////////
float BrushPanel::getRadius() const
{
	return m_radius[m_modeMenu->getSelectedItem()];
}


///////////////////////////////////////////////////////////
float BrushPanel::getStrength() const
{
	return m_strength[m_modeMenu->getSelectedItem()];
}


///////////////////////////////////////////////////////////
float BrushPanel::getGradient() const
{
	return m_gradient[m_modeMenu->getSelectedItem()];
}